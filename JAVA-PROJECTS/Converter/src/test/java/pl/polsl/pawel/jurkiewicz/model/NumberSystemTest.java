package pl.polsl.pawel.jurkiewicz.model;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Test class for NumberSystem enum class.
 * This class tests the methods of the NumberSystem enum:
 * values(), valueOf(), getBase(), and isSupported().
 * 
 * @author paweljurkiewicz
 */
public class NumberSystemTest {

    /**
     * Test of values() method, of class NumberSystem.
     * This test verifies that the values() method correctly returns 
     * an array of all enum constants in the correct order.
     * 
     * @param enumName The name of the enum constant to verify (BINARY, OCTAL, DECIMAL, HEXADECIMAL, BASE32)
     */
    @ParameterizedTest
    @ValueSource(strings = {"BINARY", "OCTAL", "DECIMAL", "HEXADECIMAL", "BASE32"})
    public void testValues(String enumName) {
        NumberSystem[] expResult = {NumberSystem.BINARY, NumberSystem.OCTAL, 
                                    NumberSystem.DECIMAL, NumberSystem.HEXADECIMAL, 
                                    NumberSystem.BASE32};

        NumberSystem[] result = NumberSystem.values();

        assertArrayEquals(expResult, result, "The values() method did not return the expected array.");
    }

    /**
     * Test of valueOf() method, of class NumberSystem.
     * This test verifies that the valueOf() method correctly converts a 
     * valid enum name (string) to the corresponding NumberSystem enum constant.
     * It also checks that an IllegalArgumentException is thrown for invalid names.
     * 
     * @param validName The valid name of the enum constant to test (BINARY, OCTAL, DECIMAL, HEXADECIMAL, BASE32)
     */
    @ParameterizedTest
    @ValueSource(strings = {"BINARY", "OCTAL", "DECIMAL", "HEXADECIMAL", "BASE32"})
    public void testValueOf(String validName) {
        NumberSystem expResult = NumberSystem.valueOf(validName);
        assertEquals(expResult, NumberSystem.valueOf(validName), "The valueOf method did not return the expected enum for valid name.");

        String invalidName = "INVALID";
        assertThrows(IllegalArgumentException.class, () -> {
            NumberSystem.valueOf(invalidName);
        }, "An IllegalArgumentException should be thrown for an invalid name.");
    }

    /**
     * Test of getBase() method, of class NumberSystem.
     * This test verifies that each enum constant returns the correct base.
     * 
     * @param base The base corresponding to each number system (2, 8, 10, 16, 32)
     */
    @ParameterizedTest
    @ValueSource(ints = {2, 8, 10, 16, 32})
    public void testGetBase(int base) {
        int[] expResult = {2, 8, 10, 16, 32};
        NumberSystem[] instance = {NumberSystem.BINARY, NumberSystem.OCTAL, 
                                   NumberSystem.DECIMAL, NumberSystem.HEXADECIMAL, 
                                   NumberSystem.BASE32};

        for (int i = 0; i < 5; i++) {
            int result = instance[i].getBase();
            assertEquals(expResult[i], result, "The base of the " + instance[i] + " system should be " + expResult[i] + ".");
        }
    }

    /**
     * Test of isSupported() method, of class NumberSystem.
     * This test verifies if a base is supported by the number system.
     * It tests both supported and unsupported base values.
     * 
     * @param base The base to test for support (2, 8, 10, 16, 32, 3, 0)
     */
    @ParameterizedTest
    @ValueSource(ints = {2, 8, 10, 16, 32, 3, 0})
    public void testIsSupported(int base) {
        // Test for unsupported bases
        if (base == 0 || base == 3) {
            assertFalse(NumberSystem.isSupported(base), "Base " + base + " should not be supported.");
        } else {
            assertTrue(NumberSystem.isSupported(base), "Base " + base + " should be supported.");
        }
    }
}
