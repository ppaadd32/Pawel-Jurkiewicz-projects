package pl.polsl.pawel.jurkiewicz.model;

/**
 * Enum representing the allowed number systems.
 * This enum defines a set of supported number systems with their respective bases.
 * It also provides a method to check if a given base is supported.
 * 
 * @author paweljurkiewicz
 */
public enum NumberSystem {
    
    /** Binary number system with base 2. */
    BINARY(2),
    
    /** Octal number system with base 8. */
    OCTAL(8),
    
    /** Decimal number system with base 10. */
    DECIMAL(10),
    
    /** Hexadecimal number system with base 16. */
    HEXADECIMAL(16),
    
    /** Base 32 number system with base 32. */
    BASE32(32); // Maximum allowed system base

    /** The base of the number system. */
    private final int base;

    /**
     * Constructor for the NumberSystem enum.
     * Initializes the enum constant with the specified base value.
     * 
     * @param base The base value of the number system.
     */
    NumberSystem(int base) {
        this.base = base;
    }

    /**
     * Gets the base of the number system.
     * 
     * @return The base of the number system.
     */
    public int getBase() {
        return base;
    }

    /**
     * Checks if a given base is supported by the enum.
     * 
     * @param base The base value to check.
     * @return true if the base is supported; false otherwise.
     */
    public static boolean isSupported(int base) {
        for (NumberSystem system : NumberSystem.values()) {
            if (system.getBase() == base) {
                return true;
            }
        }
        return false;
    }
}
