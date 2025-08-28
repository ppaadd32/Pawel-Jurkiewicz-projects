package pl.polsl.pawel.jurkiewicz.model;

import pl.polsl.pawel.jurkiewicz.GUI.ErrorWindow;

/**
 * Exception thrown when invalid input is provided for number conversion.
 * This exception is used to indicate that the input data provided to 
 * the conversion system is invalid, such as an invalid number or an unsupported base.
 *
 * @author PawelJurkiewicz
 * @version 1.0
 */
public class InvalidInputException extends Exception {

    /**
     * Constructs a new InvalidInputException with the specified detail message.
     * 
     * @param message The detail message explaining the reason for the exception.
     */
    public InvalidInputException(String message) {
        super(message);
    }

    /**
     * Displays an error message using a custom ErrorWindow UI component.
     * This method is called when the exception is caught, and it displays the 
     * provided message in an error window.
     * 
     * @param message The error message to be displayed.
     */
    public void printError(String message) {
        ErrorWindow error = new ErrorWindow(message);
        error.printE(message);
    }
}
