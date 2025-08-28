package pl.polsl.pawel.jurkiewicz.model;

import pl.polsl.pawel.jurkiewicz.GUI.NewJFrame;

/**
 * Utility class for converting numbers between various numeral systems.
 */
public class Convert {

    private ConversionHistory conversionHistory = new ConversionHistory();

    public Convert() {
        this.conversionHistory = new ConversionHistory(); // Upewnij się, że pole jest zawsze zainicjalizowane
    }

    
    
    /**
     * Konwertuje liczbę z jednego systemu liczbowego do innego.
     *
     * @param params obiekt zawierający dane wejściowe do konwersji.
     * @return wynik konwersji jako String w systemie docelowym.
     * @throws InvalidInputException jeśli wejście jest niepoprawne dla
     * wskazanego systemu wejściowego.
     */
    
    public String calculate(ConversionParameters params) throws InvalidInputException {
        try {
            // Sprawdzanie, czy systemy wejściowy i wyjściowy są obsługiwane
            if (!NumberSystem.isSupported(params.systemIn()) || !NumberSystem.isSupported(params.systemOut())) {
                throw new InvalidInputException("Dozwolone systemy liczbowe to: "
                        + "2, 8, 10, 16, 32.");
            }

            conversionHistory = NewJFrame.getHis();

            // Konwersja liczby wejściowej na system dziesiętny
            int decimalValue = Integer.parseInt(params.inputNumber(), params.systemIn());

            // Konwersja liczby dziesiętnej na docelowy system liczbowy
            String result = Integer.toString(decimalValue, params.systemOut());

            // Dodanie wyniku do historii
            if (conversionHistory != null) {
                conversionHistory.addRecord("Input: " + params.inputNumber()
                        + ", From: " + params.systemIn()
                        + ", To: " + params.systemOut()
                        + ", Result: " + result);
            } else {
                System.out.println("Conversion history is null. Skipping history update.");
            }

            // Aktualizacja historii w GUI
            NewJFrame.setHis(conversionHistory);

            return result;
        } catch (NumberFormatException e) {
            throw new InvalidInputException("Niepoprawne wejście dla systemu " + params.systemIn()
                    + ": " + params.inputNumber());
        }
    }

}
