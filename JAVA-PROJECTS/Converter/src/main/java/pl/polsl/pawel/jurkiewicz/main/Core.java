package pl.polsl.pawel.jurkiewicz.main;

import pl.polsl.pawel.jurkiewicz.model.ConvertSystem;
import pl.polsl.pawel.jurkiewicz.model.InvalidInputException;
import pl.polsl.pawel.jurkiewicz.model.ConversionParameters;
import pl.polsl.pawel.jurkiewicz.view.Results;

/**
 * Main controller class for handling the conversion logic.
 * This class processes input from the command line or interactively if arguments are missing.
 * It initiates the conversion process based on user input and displays the result.
 * 
 * @author paweljurkiewicz
 */
public class Core {

    Results res = new Results();

    /**
     * Main method to handle the conversion process.
     * This method takes three parameters from the command line:
     * 1. Number to convert,
     * 2. Input system (e.g., binary, decimal),
     * 3. Output system (e.g., hexadecimal).
     * 
     * It creates a ConversionParameters object with the given arguments and passes it
     * to the ConvertSystem class to perform the conversion.
     * Finally, the result is displayed via the Results class.
     * 
     * @param args the command-line arguments (number, input system, output system)
     */
    public void handle(String[] args) {
        try {
            ConversionParameters params = new ConversionParameters(args[0], Integer.parseInt(args[1]), Integer.parseInt(args[2]));
            
            ConvertSystem converter = new ConvertSystem();
            String result = converter.calculate(params);
            
            res.showOutput(result);
        } catch (InvalidInputException e) {
            e.printError(e.getMessage());
        }
    }
}
