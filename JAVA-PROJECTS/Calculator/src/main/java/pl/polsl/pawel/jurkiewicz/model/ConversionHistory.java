package pl.polsl.pawel.jurkiewicz.model;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import lombok.Getter;
import lombok.Setter;

/**
 * Class responsible for storing the history of conversions. This class allows
 * adding new conversion records, clearing the history, and
 * filtering/simplifying the history.
 *
 * @author paweljurkiewicz
 */
@Setter
@Getter
public class ConversionHistory {

    /**
     * List that stores the history of conversions. Each entry is a string
     * representing a single conversion record.
     */
    private List<String> history = new ArrayList<>();

    /**
     * Adds a new conversion record to the history.
     *
     * @param record A string containing the details of a conversion (e.g.,
     * "Input: 10, From: 10, To: 2, Result: 1010").
     */
    public void addRecord(String record) {
        history.add(record);
    }

    /**
     * Returns the number of records in the history.
     *
     * @return the size of the history.
     */
    public int size() {
        return history.size();
    }

    /**
     * Filters and simplifies the history based on the input system. This method
     * filters conversion records where the input system matches the provided
     * filter value, and then simplifies each record to show only the input and
     * result.
     *
     * @param systemInFilter The input system to filter the history records by
     * (e.g., 10 for decimal).
     * @return A list of simplified conversion records that match the input
     * system filter.
     */
    public List<String> filterAndSimplifyHistory(int systemInFilter) {
        // Tworzymy strumień z historii
        return history.stream()
                // Filtrujemy wpisy, które zawierają odpowiedni system wejściowy (np. "From: 10")
                .filter(entry -> {
                    // Dzielimy wpis na części i wyciągamy wartość systemu wejściowego
                    String[] parts = entry.split(", ");
                    // Parsujemy system wejściowy (From) jako liczbę
                    int fromSystem = Integer.parseInt(parts[1].split(": ")[1]);
                    // Porównujemy z podanym systemem
                    return fromSystem == systemInFilter;
                })
                // Upraszczamy każdy wpis
                .map(this::simplifyEntry)
                // Zbieramy do listy
                .collect(Collectors.toList());
    }

    /**
     * Simplifies a single conversion record to a format showing only the input
     * and result.
     *
     * @param entry The full conversion record string (e.g., "Input: 10, From:
     * 10, To: 2, Result: 1010").
     * @return A simplified string representation (e.g., "10 -> 1010").
     */
    private String simplifyEntry(String entry) {
        // Example parsing: "Input: 10, From: 10, To: 2, Result: 1010" -> "10 -> 1010"
        String[] parts = entry.split(", ");
        String input = parts[0].split(": ")[1];
        String result = parts[3].split(": ")[1];
        return input + " -> " + result;
    }
}
