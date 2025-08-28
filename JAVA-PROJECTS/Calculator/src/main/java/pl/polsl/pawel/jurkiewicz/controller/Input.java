/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package pl.polsl.pawel.jurkiewicz.controller;

import java.util.Scanner;

/**
 * Controller class for handling input from the user. Provides methods for
 * gathering input data.
 * 
 * @author PawelJurkiewicz
 * @version 1.0
 */
public class Input {

    /**
     * Default constructor for Input.
     */
    public Input() {
        // Default constructor
    }

    /**
     * Gets input from the user.
     *
     * @return input as a string.
     */
    public String getInput() {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter the number: ");
        return scanner.nextLine();
    }

    /**
     * Gets the numeric system from the user.
     *
     * @param message the message to display to the user.
     * @return the numeric system as an integer (2 or 10).
     */
    public int getSystem(String message) {
        Scanner scanner = new Scanner(System.in);
        System.out.print(message);
        return scanner.nextInt();
    }
}
