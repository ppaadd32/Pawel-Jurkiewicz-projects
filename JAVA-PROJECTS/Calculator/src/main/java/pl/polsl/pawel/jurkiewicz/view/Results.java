package pl.polsl.pawel.jurkiewicz.view;

import pl.polsl.pawel.jurkiewicz.GUI.ErrorWindow;
import pl.polsl.pawel.jurkiewicz.model.ConversionHistory;

/**
 * The Results class represents the View component responsible for displaying conversion results to the user.
 *
 * It provides methods to show the results of numeral system conversions and interact with the application's 
 * error-handling and history storage components.
 * 
 * @author PawelJurkiewicz
 */
public class Results {

    /**
     * The last output result to be displayed to the user.
     */
    private static String output;

    /**
     * The error window object for displaying error messages.
     */
    private ErrorWindow error;

    /**
     * The conversion history manager for storing past conversion records.
     */
    private ConversionHistory his = new ConversionHistory();

    /**
     * Default constructor for the Results class.
     */
    public Results() {
        // Default constructor body
    }

    /**
     * Retrieves the last displayed result.
     *
     * @return the last conversion result as a String.
     */
    public static String getOut() {
        return output;
    }

    /**
     * Displays the result of a numeral system conversion.
     * This method updates the current output and can be extended to integrate with the error window.
     *
     * @param result the conversion result to be displayed.
     */
    public void showOutput(String result) {
        output = result;
        // Placeholder for potential integration with ErrorWindow
        // error.setResult(result);
    }
}
