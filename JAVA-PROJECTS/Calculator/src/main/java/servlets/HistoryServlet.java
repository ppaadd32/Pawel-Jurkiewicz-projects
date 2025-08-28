package servlets;

import java.io.IOException;
import java.io.PrintWriter;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.ServletContext;
import pl.polsl.pawel.jurkiewicz.model.ConversionHistory;
import jakarta.servlet.http.Cookie;

/**
 * Servlet for displaying the conversion history and the most recent conversion result.
 * This servlet retrieves the conversion history and displays the last result from a cookie.
 * 
 * @author Your Name
 * @version 1.0
 */
@WebServlet(urlPatterns = {"/history"})
public class HistoryServlet extends HttpServlet {

    /**
     * Handles GET requests for displaying conversion history and the last conversion result.
     * 
     * @param request the HTTP request.
     * @param response the HTTP response.
     * @throws ServletException if an error occurs during request processing.
     * @throws IOException if an I/O error occurs while writing the response.
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        handleRequest(request, response);
    }

    /**
     * Handles POST requests for displaying conversion history and the last conversion result.
     * 
     * @param request the HTTP request.
     * @param response the HTTP response.
     * @throws ServletException if an error occurs during request processing.
     * @throws IOException if an I/O error occurs while writing the response.
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        handleRequest(request, response);
    }

    /**
     * Retrieves and displays the full conversion history, as well as the most recent conversion result stored in a cookie.
     * 
     * @param request the HTTP request.
     * @param response the HTTP response.
     * @throws ServletException if an error occurs during request processing.
     * @throws IOException if an I/O error occurs while writing the response.
     */
    private void handleRequest(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        ServletContext context = getServletContext();
        ConversionHistory history = (ConversionHistory) context.getAttribute("history");

        response.setContentType("text/html");

        try (PrintWriter out = response.getWriter()) {
            out.println("<html>");
            out.println("<head><title>Conversion History</title></head>");
            out.println("<body>");
            out.println("<h1>Conversion History</h1>");

            // Retrieve the "lastResult" cookie
            Cookie[] cookies = request.getCookies();
            String lastResult = null;

            out.println("<h2>Last Conversion Result (1 hour):</h2>");

            if (cookies != null) {
                for (Cookie cookie : cookies) {
                    if ("lastResult".equals(cookie.getName())) {
                        lastResult = cookie.getValue();
                        out.println("<p>" + lastResult + "</p>");
                        break;
                    }
                }
            }

            // Display the last conversion result
            if (lastResult == null) {
                out.println("<p>No recent conversion result found.</p>");
            }

            // Display the full conversion history (newest to oldest)
            out.println("<h2>Full Conversion History (newest to oldest):</h2>");
            if (history != null && !history.getHistory().isEmpty()) {
                for (int i = history.size() - 1; i >= 0; i--) {
                    out.println("<p>" + history.getHistory().get(i) + "</p>");
                }
            } else {
                out.println("<p>Conversion history is empty.</p>");
            }

            out.println("</body>");
            out.println("</html>");
        }
    }
}
