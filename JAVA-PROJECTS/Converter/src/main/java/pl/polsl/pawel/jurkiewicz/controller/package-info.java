/**
 * This package contains the controller components for managing user input in the application.
 *
 * The main class, {@link pl.polsl.pawel.jurkiewicz.controller.Input}, is responsible for collecting input
 * from the user and validating it as required. It provides methods for retrieving the number to convert
 * and specifying the numerical system used for conversion.
 *
 * Key functionality of this package:
 * <ul>
 *   <li>{@link pl.polsl.pawel.jurkiewicz.controller.Input#getInput()}: Prompts the user to enter a value to be converted,
 *       and returns the input as a string for further processing.</li>
 *   <li>{@link pl.polsl.pawel.jurkiewicz.controller.Input#getSystem(String)}: Requests the user to specify a numeral
 *       system (e.g., binary or decimal), which serves as input or output for the conversion process.</li>
 * </ul>
 *
 * This package plays a critical role within the MVC architecture by facilitating user input collection, which is
 * essential for the application's controller layer. It ensures a smooth interaction between the user, view, and model
 * components by providing a reliable interface for data entry.
 * 
 * @author PawelJurkiewicz
 * @version 1.0
 * @since 1.0
 * @see pl.polsl.pawel.jurkiewicz.controller.Input
 */
package pl.polsl.pawel.jurkiewicz.controller;
