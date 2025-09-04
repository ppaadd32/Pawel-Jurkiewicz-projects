package pl.polsl.pawel.jurkiewicz.model;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;
import static org.junit.jupiter.api.Assertions.*;

/**
 *
 * @author paweljurkiewicz
 */
public class ConvertSystemTest {

    /**
     * Test of calculate method, of class ConvertSystem.
     * This test is parameterized to run multiple cases with different input and expected results.
     * It tests conversion between various number systems.
     * 
     * @param input The input number as a string.
     * @param fromBase The base of the input number.
     * @param toBase The base to convert the number to.
     * @param expectedResult The expected result after conversion.
     */
    @ParameterizedTest
    @CsvSource({
        "'13', 10, 2, '1101'",  // Decimal to Binary
        "'13', 10, 8, '15'",    // Decimal to Octal
        "'13', 10, 16, 'd'",    // Decimal to Hexadecimal
        "'1101', 2, 10, '13'",  // Binary to Decimal
        "'1101', 2, 8, '15'",   // Binary to Octal
        "'1101', 2, 16, 'd'",   // Binary to Hexadecimal
        "'15', 8, 10, '13'",    // Octal to Decimal
        "'15', 8, 2, '1101'",   // Octal to Binary
        "'D', 16, 10, '13'",    // Hexadecimal to Decimal
        "'D', 16, 2, '1101'",   // Hexadecimal to Binary
        "'1', 32, 10, '1'",     // Base 32 to Decimal
        "'1', 32, 2, '1'"       // Base 32 to Binary
    })
    public void testCalculate(String input, int fromBase, int toBase, String expectedResult) throws Exception {
        System.out.println("calculate");

        ConversionParameters params = new ConversionParameters(input, fromBase, toBase);
        ConvertSystem instance = new ConvertSystem();

        String result = instance.calculate(params);
        assertEquals(expectedResult, result, String.format("%s (Base %d) should be converted to %s (Base %d).", input, fromBase, expectedResult, toBase));
    }
}
