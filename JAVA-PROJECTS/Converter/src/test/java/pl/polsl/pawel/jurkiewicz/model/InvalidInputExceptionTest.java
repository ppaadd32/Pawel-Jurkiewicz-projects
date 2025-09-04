package pl.polsl.pawel.jurkiewicz.model;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;
import static org.junit.jupiter.api.Assertions.*;

import pl.polsl.pawel.jurkiewicz.GUI.ErrorWindow;

/**
 * Test class for InvalidInputException
 * This test class checks the behavior of the InvalidInputException, specifically the printError method.
 * 
 * @author paweljurkiewicz
 */
public class InvalidInputExceptionTest {
    
    /**
     * Parameterized test for the printError method of InvalidInputException.
     * This test checks if the error message is correctly passed and printed.
     * 
     * @param message The error message to be printed.
     */
    @ParameterizedTest
    @ValueSource(strings = {
        "Invalid input for conversion", 
        "Conversion failed due to invalid format", 
        "Input value is out of range"
    })
    public void testPrintError(String message) {
        ErrorWindow errorWindow = new ErrorWindow(message);

        InvalidInputException exception = new InvalidInputException(message);
        exception.printError(message);

        assertEquals(errorWindow.getPrintedMessage(), message, "Error message should match expected output.");
    }
}
