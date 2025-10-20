package com.mycompany.passwordgenerator;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.datatransfer.StringSelection;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * @brief Desktop password generator using Java Swing with strength meter.
 *
 * Provides a simple GUI to generate passwords with configurable character sets,
 * length, and an option to exclude visually similar characters. Displays a
 * projected strength indicator and allows copying to clipboard.
 *
 * @author
 *   Paweł Jurkiewicz
 * @version 1.0
 * @date 2025-10-20
 * @since 1.0
 */
public class PasswordGenerator extends JFrame {
    /** Output field for the generated password (read-only). */
    private final JTextField outputField = new JTextField();
    /** Spinner to choose password length (min 4, max 128). */
    private final JSpinner lengthSpinner = new JSpinner(new SpinnerNumberModel(16, 4, 128, 1));
    /** Toggle for lowercase letters a–z. */
    private final JCheckBox lowerCb = new JCheckBox("a–z", true);
    /** Toggle for uppercase letters A–Z. */
    private final JCheckBox upperCb = new JCheckBox("A–Z", true);
    /** Toggle for digits 0–9. */
    private final JCheckBox digitsCb = new JCheckBox("0–9", true);
    /** Toggle for symbols set. */
    private final JCheckBox symbolsCb = new JCheckBox("!@#$…", true);
    /** Toggle to exclude visually similar characters (0,O,o,l,1,I,|). */
    private final JCheckBox excludeSimilarCb = new JCheckBox("Exclude similar (0 O o l 1 I |)");
    /** Button to generate a new password. */
    private final JButton generateBtn = new JButton("Generate (Enter)");
    /** Button to copy the password to clipboard. */
    private final JButton copyBtn = new JButton("Copy");
    /** Button to clear the output field. */
    private final JButton clearBtn = new JButton("Clear");
    /** Strength progress bar (0–100). */
    private final JProgressBar strengthBar = new JProgressBar(0, 100);
    /** Label showing qualitative strength category. */
    private final JLabel strengthLabel = new JLabel("Strength: –");
    /** Label showing numeric percentage below the progress bar. */
    private final JLabel strengthPctLabel = new JLabel("0%");

    private static final String LOWER = "abcdefghijklmnopqrstuvwxyz";
    private static final String UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    private static final String DIGITS = "0123456789";
    private static final String SYMBOLS = "!@#$%^&*()-_=+[]{};:,.<>?/\\`~";
    private static final String SIMILAR = "0OoIl1|";

    private final SecureRandom rng = new SecureRandom();

    /**
     * @brief Constructs the Swing UI and wires all actions.
     */
    public PasswordGenerator() {
        super("Password Generator — Java Swing");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setMinimumSize(new Dimension(560, 330));
        setLocationByPlatform(true);
        setLayout(new BorderLayout());

        // Top (output)
        outputField.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 18));
        outputField.setEditable(false);
        outputField.setToolTipText("Generated password");
        outputField.setBorder(BorderFactory.createCompoundBorder(
                new EmptyBorder(10, 10, 10, 10),
                outputField.getBorder()
        ));
        add(outputField, BorderLayout.NORTH);

        // Center (options)
        JPanel center = new JPanel(new GridBagLayout());
        center.setBorder(new EmptyBorder(0, 10, 0, 10));
        GridBagConstraints c = new GridBagConstraints();
        c.insets = new Insets(6, 6, 6, 6);
        c.fill = GridBagConstraints.HORIZONTAL;

        // Length
        c.gridx = 0; c.gridy = 0; c.weightx = 0;
        center.add(new JLabel("Length:"), c);
        c.gridx = 1; c.weightx = 1;
        lengthSpinner.setToolTipText("Password length (4–128)");
        center.add(lengthSpinner, c);

        JPanel sets = new JPanel(new FlowLayout(FlowLayout.LEFT, 8, 0));
        sets.add(lowerCb);
        sets.add(upperCb);
        sets.add(digitsCb);
        sets.add(symbolsCb);

        c.gridx = 0; c.gridy = 1; c.weightx = 0;
        center.add(new JLabel("Character sets:"), c);
        c.gridx = 1; c.weightx = 1;
        center.add(sets, c);

        c.gridx = 1; c.gridy = 2; c.weightx = 1;
        center.add(excludeSimilarCb, c);

        JPanel strengthPanel = new JPanel(new BorderLayout(8, 0));
        strengthBar.setStringPainted(false); // we use a separate label below the bar

        JPanel barWithPct = new JPanel(new BorderLayout());
        barWithPct.add(strengthBar, BorderLayout.CENTER);
        strengthPctLabel.setHorizontalAlignment(SwingConstants.CENTER);
        strengthPctLabel.setBorder(new EmptyBorder(4, 0, 0, 0)); // spacing below the bar
        barWithPct.add(strengthPctLabel, BorderLayout.SOUTH);

        strengthPanel.add(barWithPct, BorderLayout.CENTER);
        strengthPanel.add(strengthLabel, BorderLayout.EAST);

        c.gridx = 0; c.gridy = 3; c.weightx = 0;
        center.add(new JLabel("Strength:"), c);
        c.gridx = 1; c.weightx = 1;
        center.add(strengthPanel, c);

        add(center, BorderLayout.CENTER);

        JPanel bottom = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        bottom.setBorder(new EmptyBorder(0, 10, 10, 10));
        bottom.add(clearBtn);
        bottom.add(copyBtn);
        bottom.add(generateBtn);
        add(bottom, BorderLayout.SOUTH);

        generateBtn.addActionListener(e -> onGenerate());
        copyBtn.addActionListener(e -> onCopy());
        clearBtn.addActionListener(e -> {
            outputField.setText("");
            updateStrength("", 0);
        });

        getRootPane().setDefaultButton(generateBtn);

        Runnable onOptionsChange = this::updateProjectedStrengthFromOptions;
        lowerCb.addActionListener(e -> onOptionsChange.run());
        upperCb.addActionListener(e -> onOptionsChange.run());
        digitsCb.addActionListener(e -> onOptionsChange.run());
        symbolsCb.addActionListener(e -> onOptionsChange.run());
        excludeSimilarCb.addActionListener(e -> onOptionsChange.run());
        lengthSpinner.addChangeListener(e -> onOptionsChange.run());

        bindShortcut("ctrl C", copyBtn, KeyEvent.VK_C, Toolkit.getDefaultToolkit().getMenuShortcutKeyMaskEx());
        bindShortcut("ctrl G", generateBtn, KeyEvent.VK_G, Toolkit.getDefaultToolkit().getMenuShortcutKeyMaskEx());

        updateProjectedStrengthFromOptions();
    }

    /**
     * @brief Binds a keyboard shortcut to a button's action listeners.
     *
     * @param name logical action name used in the InputMap/ActionMap
     * @param button target button to trigger
     * @param key KeyEvent VK_* code
     * @param mask platform shortcut mask (e.g., Ctrl/Cmd)
     */
    private void bindShortcut(String name, JButton button, int key, int mask) {
        button.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(KeyStroke.getKeyStroke(key, mask), name);
        button.getActionMap().put(name, new AbstractAction() {
            @Override public void actionPerformed(java.awt.event.ActionEvent e) {
                for (ActionListener al : button.getActionListeners()) {
                    al.actionPerformed(e);
                }
            }
        });
    }

    /**
     * @brief Handles password generation click/Enter.
     *
     * Builds the alphabet from selected options, generates a password,
     * updates the output field, and refreshes the strength indicator.
     */
    private void onGenerate() {
        int len = (Integer) lengthSpinner.getValue();
        String alphabet = buildAlphabet();
        if (alphabet.isEmpty()) {
            JOptionPane.showMessageDialog(this,
                    "Select at least one character set.",
                    "No characters", JOptionPane.WARNING_MESSAGE);
            return;
        }
        String pwd = generatePassword(len, alphabet);
        outputField.setText(pwd);
        updateStrength(pwd, alphabet.length());
    }

    /**
     * @brief Copies the generated password to system clipboard.
     */
    private void onCopy() {
        String text = outputField.getText();
        if (text == null || text.isEmpty()) {
            JOptionPane.showMessageDialog(this,
                    "Generate a password first.",
                    "No password", JOptionPane.INFORMATION_MESSAGE);
            return;
        }
        Toolkit.getDefaultToolkit()
                .getSystemClipboard()
                .setContents(new StringSelection(text), null);
        JOptionPane.showMessageDialog(this, "Copied to clipboard.");
    }

    /**
     * @brief Builds the candidate alphabet based on selected character sets.
     *
     * If "Exclude similar" is enabled, visually similar characters are removed.
     *
     * @return String with all allowed characters (may be empty).
     */
    private String buildAlphabet() {
        StringBuilder sb = new StringBuilder();
        if (lowerCb.isSelected()) sb.append(LOWER);
        if (upperCb.isSelected()) sb.append(UPPER);
        if (digitsCb.isSelected()) sb.append(DIGITS);
        if (symbolsCb.isSelected()) sb.append(SYMBOLS);

        String alphabet = sb.toString();
        if (excludeSimilarCb.isSelected()) {
            StringBuilder filtered = new StringBuilder();
            outer: for (char ch : alphabet.toCharArray()) {
                for (char s : SIMILAR.toCharArray()) {
                    if (ch == s) continue outer;
                }
                filtered.append(ch);
            }
            alphabet = filtered.toString();
        }
        return alphabet;
    }

    /**
     * @brief Generates a password of given length using the given alphabet.
     *
     * Ensures at least one character from each selected set is included.
     *
     * @param length desired password length
     * @param alphabet final alphabet to draw characters from
     * @return generated password string
     */
    private String generatePassword(int length, String alphabet) {
        List<Character> result = new ArrayList<>(length);

        List<String> requiredSets = new ArrayList<>();
        if (lowerCb.isSelected()) requiredSets.add(filterSet(LOWER));
        if (upperCb.isSelected()) requiredSets.add(filterSet(UPPER));
        if (digitsCb.isSelected()) requiredSets.add(filterSet(DIGITS));
        if (symbolsCb.isSelected()) requiredSets.add(filterSet(SYMBOLS));

        for (String set : requiredSets) {
            if (!set.isEmpty()) {
                result.add(randomChar(set));
            }
        }

        for (int i = result.size(); i < length; i++) {
            result.add(randomChar(alphabet));
        }

        Collections.shuffle(result, rng);

        StringBuilder pwd = new StringBuilder(length);
        for (char ch : result) pwd.append(ch);
        return pwd.toString();
    }

    /**
     * @brief Filters a character set according to the "exclude similar" option.
     *
     * @param set input character set
     * @return filtered set (or original if exclusion disabled)
     */
    private String filterSet(String set) {
        if (!excludeSimilarCb.isSelected()) return set;
        StringBuilder filtered = new StringBuilder();
        outer: for (char ch : set.toCharArray()) {
            for (char s : SIMILAR.toCharArray()) {
                if (ch == s) continue outer;
            }
            filtered.append(ch);
        }
        return filtered.toString();
    }

    /**
     * @brief Picks a random character from the given source string.
     *
     * @param source non-empty string to sample
     * @return randomly selected character
     */
    private char randomChar(String source) {
        int idx = rng.nextInt(source.length());
        return source.charAt(idx);
    }

    /**
     * @brief Updates the strength UI based on the actual password.
     *
     * Entropy approximation: L * log2(N), where L is length,
     * and N is the size of the alphabet used.
     *
     * @param password current password (may be empty)
     * @param alphabetSize number of unique characters in the alphabet
     */
    private void updateStrength(String password, int alphabetSize) {
        double entropy = (password == null) ? 0 : password.length() * log2(Math.max(1, alphabetSize));
        int pct = strengthPercentage(entropy);
        strengthBar.setValue(pct);
        strengthPctLabel.setText(pct + "%");
        strengthLabel.setText("Strength: " + labelForEntropy(entropy));
    }

    /**
     * @brief Updates the projected (pre-generation) strength indicator.
     *
     * Uses current options (length and sets) to compute a predicted entropy.
     */
    private void updateProjectedStrengthFromOptions() {
        int len = (Integer) lengthSpinner.getValue();
        int N = Math.max(1, buildAlphabet().length());
        double entropy = len * log2(N);
        int pct = strengthPercentage(entropy);
        strengthBar.setValue(pct);
        strengthPctLabel.setText(pct + "%");
        strengthLabel.setText("Strength (projected)");
    }

    /**
     * @brief Log base 2 helper.
     * @param x positive number
     * @return log2(x)
     */
    private static double log2(double x) {
        return Math.log(x) / Math.log(2.0);
    }

    /**
     * @brief Maps entropy (bits) to a human-readable category.
     * @param e entropy in bits
     * @return "Weak", "Medium", "Strong" or "Very strong"
     */
    private static String labelForEntropy(double e) {
        if (e < 40) return "Weak";
        if (e < 60) return "Medium";
        if (e < 80) return "Strong";
        return "Very strong";
    }

    /**
     * @brief Converts entropy to a 0–100 percentage for the progress bar.
     *
     * Values above 100 bits are capped at 100%.
     *
     * @param e entropy in bits
     * @return integer percentage in range [0,100]
     */
    private static int strengthPercentage(double e) {
        double capped = Math.min(100.0, e);
        return (int) Math.round(capped);
    }

    /**
     * @brief Entry point. Sets system Look & Feel and shows the window.
     *
     * @param args CLI arguments (unused)
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            setSystemLAF();
            PasswordGenerator app = new PasswordGenerator();
            app.setVisible(true);
        });
    }

    /**
     * @brief Attempts to set the platform's native Look & Feel.
     */
    private static void setSystemLAF() {
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception ignored) {}
    }
}
