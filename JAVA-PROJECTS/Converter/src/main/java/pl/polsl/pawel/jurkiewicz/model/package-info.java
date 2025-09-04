/**
 * This package provides the model components for handling numeral system conversions in the application.
 *
 * It includes two main classes:
 * <ul>
 *   <li>{@link pl.polsl.pawel.jurkiewicz.model.Convert}: A utility class that offers methods for converting numbers
 *       between various numeral systems, such as binary, octal, decimal, hexadecimal, and others. The method
 *       {@link pl.polsl.pawel.jurkiewicz.model.Convert#calculate(String, int, int)} performs the conversion
 *       based on the input number and the specified input and output bases, enabling flexible conversions between
 *       different numeral systems.</li>
 *   <li>{@link pl.polsl.pawel.jurkiewicz.model.InvalidInputException}: A custom exception class that signals an error
 *       when invalid input is provided for conversion. This exception is used to handle issues such as incorrect
 *       formats or unsupported characters in the input number, ensuring robust error handling and validation in the application.</li>
 * </ul>
 *
 * These classes are central to the core functionality of the application, enabling the conversion of numbers
 * between various numeral systems and handling input validation. They form the model layer of the MVC architecture,
 * contributing to the application's data handling and business logic.
 *
 * @author PawelJurkiewicz
 * @version 1.0
 * @since 1.0
 * @see pl.polsl.pawel.jurkiewicz.model.Convert
 * @see pl.polsl.pawel.jurkiewicz.model.InvalidInputException
 */
package pl.polsl.pawel.jurkiewicz.model;
