package pl.polsl.pawel.jurkiewicz.model;

/**
 * A record storing the input data for a conversion: the input number, 
 * the input system (base), and the output system (base).
 * This record is used to encapsulate the parameters needed for converting 
 * a number from one base to another.
 * 
 * @param inputNumber The number to be converted, represented as a string.
 * @param systemIn The base of the input number (e.g., 10 for decimal, 2 for binary).
 * @param systemOut The base of the desired output number (e.g., 16 for hexadecimal, 2 for binary).
 */
public record ConversionParameters(String inputNumber, int systemIn, int systemOut) {
}
