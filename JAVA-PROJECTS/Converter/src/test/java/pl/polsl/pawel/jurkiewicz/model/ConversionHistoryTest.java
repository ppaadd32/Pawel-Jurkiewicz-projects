/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/UnitTests/JUnit5TestClass.java to edit this template
 */
package pl.polsl.pawel.jurkiewicz.model;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.*;
import java.util.List;
import java.util.ArrayList;

/**
 *
 * @author paweljurkiewicz
 */
public class ConversionHistoryTest {

    /**
     * Test the addRecord method of the ConversionHistory class.
     * It checks if the record is added to the history correctly.
     * 
     * @param record The conversion record to be added to the history.
     */
    @ParameterizedTest
    @ValueSource(strings = {
        "Input: 10, From: 10, To: 2, Result: 1010",
        "Input: 255, From: 10, To: 16, Result: FF",
        "Input: 1010, From: 2, To: 10, Result: 10"
    })
    void testAddRecord(String record) {
        ConversionHistory instance = new ConversionHistory();

        instance.addRecord(record);

        List<String> history = instance.getHistory();
        assertEquals(1, history.size(), "The history should contain exactly 1 record.");
        assertEquals(record, history.get(0), "The added record in the history is incorrect.");
    }

    /**
     * Test the filterAndSimplifyHistory method of the ConversionHistory class.
     * It checks if the history can be filtered and simplified correctly based on the input system.
     * 
     * @param filterValue The input system to filter by (e.g., base 10, base 2).
     * @param record The record to be added to the history.
     * @param expected The expected simplified record after filtering.
     */
    @ParameterizedTest
    @CsvSource({
        "10, 'Input: 10, From: 10, To: 2, Result: 1010', '10 -> 1010'",
        "2, 'Input: 101, From: 2, To: 10, Result: 5', '101 -> 5'",
        "10, 'Input: 15, From: 10, To: 2, Result: 1111', '15 -> 1111'"
    })
    void testFilterAndSimplifyHistory(int filterValue, String record, String expected) {

        ConversionHistory instance = new ConversionHistory();

        instance.addRecord(record);

        List<String> result = instance.filterAndSimplifyHistory(filterValue);
        List<String> expResult = new ArrayList<>();
        expResult.add(expected);

        assertEquals(expResult, result, "Result of filtering and simplifying history is incorrect.");
    }

    /**
     * Test the setHistory method of the ConversionHistory class.
     * It checks if the history list can be correctly set.
     * 
     * @param history1 The first record to be added to the history.
     * @param history2 The second record to be added to the history.
     */
    @ParameterizedTest
    @CsvSource({
        "'Input: 10, From: 10, To: 2, Result: 1010', 'Input: 20, From: 10, To: 2, Result: 10100'",
        "'Input: 5, From: 2, To: 10, Result: 101', 'Input: 7, From: 2, To: 10, Result: 111'"
    })
    void testSetHistory(String history1, String history2) {
        List<String> history = new ArrayList<>();
        history.add(history1);
        history.add(history2);

        ConversionHistory instance = new ConversionHistory();

        instance.setHistory(history);

        List<String> result = instance.getHistory();
        assertEquals(history, result, "The history has not been set up correctly.");
    }

    /**
     * Test the getHistory method of the ConversionHistory class.
     * It checks if the method returns the correct history and its size.
     * 
     * @param record The record to be added to the history.
     * @param expectedSize The expected size of the history.
     */
    @ParameterizedTest
    @CsvSource({
        "'Input: 10, From: 10, To: 2, Result: 1010', 1",
        "'Input: 255, From: 10, To: 16, Result: FF', 1",
        "'Input: 1010, From: 2, To: 10, Result: 10', 1"
    })
    void testGetHistory(String record, int expectedSize) {

        ConversionHistory instance = new ConversionHistory();
        instance.addRecord(record);

        List<String> result = instance.getHistory();
        assertNotNull(result, "The history should not be null.");
        assertEquals(expectedSize, result.size(), "The history should contain the correct number of records.");
        assertEquals(record, result.get(0), "The record in the history is incorrect.");
    }

}