package pl.polsl.pawel.jurkiewicz.model;

import pl.polsl.pawel.jurkiewicz.GUI.NewJFrame;

/**
 * Utility class for converting numbers between various numeral systems.
 * 
 * @author PawelJurkiewicz

 */
public class Convert {

    private ConversionHistory conversionHistory = new ConversionHistory();

    public Convert() {
        this.conversionHistory = new ConversionHistory(); // Upewnij się, że pole jest zawsze zainicjalizowane
    }

    
    
    /**
     * Converts a number from one numeral system to another.
     *
     * @param params an object containing the input data for conversion.
     * @return the result of the conversion as a String in the target numeral system.
     * @throws InvalidInputException if the input is invalid for
     * the specified source numeral system.
     */

    
    public String calculate(ConversionParameters params) throws InvalidInputException {
        try {
            if (!NumberSystem.isSupported(params.systemIn()) || !NumberSystem.isSupported(params.systemOut())) {
                throw new InvalidInputException("Allowed Systems: "
                        + "2, 8, 10, 16, 32.");
            }

            conversionHistory = NewJFrame.getHis();

            int decimalValue = Integer.parseInt(params.inputNumber(), params.systemIn());

            String result = Integer.toString(decimalValue, params.systemOut());

            if (conversionHistory != null) {
                conversionHistory.addRecord("Input: " + params.inputNumber()
                        + ", From: " + params.systemIn()
                        + ", To: " + params.systemOut()
                        + ", Result: " + result);
            } else {
                System.out.println("Conversion history is null. Skipping history update.");
            }

            NewJFrame.setHis(conversionHistory);

            return result;
        } catch (NumberFormatException e) {
            throw new InvalidInputException("Niepoprawne wejście dla systemu " + params.systemIn()
                    + ": " + params.inputNumber());
        }
    }

}
