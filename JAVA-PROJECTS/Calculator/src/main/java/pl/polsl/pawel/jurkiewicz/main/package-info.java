/**
 * This package contains the main class responsible for initiating the application's conversion process.
 *
 * The primary class, {@link main.Main}, acts as the core controller for the program, enabling conversions
 * between different numeral systems. It manages input processing, initiates the conversion using the 
 * {@link pl.polsl.pawel.jurkiewicz.model.Convert} utility, and displays results via {@link pl.polsl.pawel.jurkiewicz.view.Results}.
 *
 * {@link main.Main} works closely with several classes:
 * <ul>
 *   <li>{@link pl.polsl.pawel.jurkiewicz.controller.Input}: Used to receive and validate user inputs, either through command-line arguments or interactive prompts.</li>
 *   <li>{@link pl.polsl.pawel.jurkiewicz.model.Convert}: Provides the core logic for number base conversions between any two numeral systems.</li>
 *   <li>{@link pl.polsl.pawel.jurkiewicz.model.InvalidInputException}: Handles invalid input scenarios and signals errors in conversion requests.</li>
 *   <li>{@link pl.polsl.pawel.jurkiewicz.view.Results}: Displays conversion results to the user in a clear format.</li>
 * </ul>
 *
 * Together, these classes follow the MVC architecture to create a streamlined, user-friendly experience,
 * where {@code Main} serves as the primary controller that connects the input handling, conversion logic,
 * and result display. This demonstrates the use of {@code package} and {@code import} in Java to modularize
 * functionality and organize code effectively.
 *
 * @author PawelJurkiewicz
 * @version 1.0
 * @since 1.0
 * @see main.Main
 */
package pl.polsl.pawel.jurkiewicz.main;
