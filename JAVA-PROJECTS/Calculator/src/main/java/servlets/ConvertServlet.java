package servlets;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.Persistence;
import jakarta.persistence.PersistenceException;
import java.io.IOException;
import java.io.PrintWriter;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.ServletContext;
import java.util.logging.Level;
import java.util.logging.Logger;
import pl.polsl.pawel.jurkiewicz.model.ConversionHistory;
import pl.polsl.pawel.jurkiewicz.model.ConversionParameters;
import pl.polsl.pawel.jurkiewicz.model.Convert;
import pl.polsl.pawel.jurkiewicz.model.InvalidInputException;
import pl.polsl.pawel.jurkiewicz.entities.Results;

import jakarta.servlet.http.Cookie;

/**
 * Servlet for handling number conversion logic and storing conversion history.
 * This servlet processes conversion requests, performs the conversion calculation,
 * and stores the result in the conversion history.
 * 
 * @author Your Name
 * @version 1.0
 */
@WebServlet(urlPatterns = {"/cos"})
public class ConvertServlet extends HttpServlet {

    
   

    void persistObject(Object object) {
        EntityManagerFactory emf = Persistence.createEntityManagerFactory("test");
        EntityManager em = emf.createEntityManager();
        em.getTransaction().begin();
        try {
            em.persist(object);
            em.getTransaction().commit();
        } catch (PersistenceException e) {
            e.printStackTrace(); // replace with proper message for the client
            em.getTransaction().rollback();
        } finally {
            em.close();
        }
    }    
    
    
    
    /**
     * Initializes the servlet by creating the shared model objects if they don't exist.
     * This is done only once for the lifecycle of the application.
     * 
     * @throws ServletException if servlet initialization fails.
     */
    @Override
    public void init() throws ServletException {
        super.init();
        ServletContext context = getServletContext();

        // Create the Convert and ConversionHistory objects if they do not exist
        if (context.getAttribute("converter") == null) {
            context.setAttribute("converter", new Convert());
        }
        if (context.getAttribute("history") == null) {
            context.setAttribute("history", new ConversionHistory());
        }
    }

    /**
     * Handles GET requests for conversion processing. Calls the {@link #handleRequest(HttpServletRequest, HttpServletResponse)} method.
     * 
     * @param request the HTTP request.
     * @param response the HTTP response.
     * @throws ServletException if request processing fails.
     * @throws IOException if an I/O error occurs.
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        handleRequest(request, response);
    }

    /**
     * Handles POST requests for conversion processing. Calls the {@link #handleRequest(HttpServletRequest, HttpServletResponse)} method.
     * 
     * @param request the HTTP request.
     * @param response the HTTP response.
     * @throws ServletException if request processing fails.
     * @throws IOException if an I/O error occurs.
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        handleRequest(request, response);
    }

    /**
     * Processes the conversion request. Validates input data, performs the conversion, 
     * and stores the result in the history. Also manages cookies for storing the last conversion result.
     * 
     * @param request the HTTP request.
     * @param response the HTTP response.
     * @throws ServletException if an error occurs during request processing.
     * @throws IOException if an I/O error occurs while writing the response.
     */
    private void handleRequest(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        ServletContext context = getServletContext();
        Convert converter = (Convert) context.getAttribute("converter");
        ConversionHistory history = (ConversionHistory) context.getAttribute("history");

        String number = request.getParameter("number");
        String inputSystemStr = request.getParameter("input-system");
        String outputSystemStr = request.getParameter("output-system");

        response.setContentType("text/html");
        try (PrintWriter out = response.getWriter()) {
            out.println("<html>");
            out.println("<head><title>Conversion Result</title></head>");
            out.println("<body>");
            out.println("<h1>Converted Result</h1>");

            try {
                int inputSystem = Integer.parseInt(inputSystemStr);
                int outputSystem = Integer.parseInt(outputSystemStr);

                if (number == null || number.isEmpty()) {
                    out.println("<p>Error: No number provided for conversion.</p>");
                } else {
                    ConversionParameters params = new ConversionParameters(number, inputSystem, outputSystem);
                    String result = converter.calculate(params);

                    // Store the result in the history
                    String record = String.format("Input: %s, From: %d, To: %d, Result: %s", number, inputSystem, outputSystem, result);
                    history.addRecord(record);

                    // Display the conversion result
                    out.println("<p>Converted number: " + result + "</p>");
                    
                    Results r = new Results();
                    r.setR("test");
                    persistObject(r);

                    // Store the result in a cookie to be used in the next request
                    Cookie resultCookie = new Cookie("lastResult", result);
                    resultCookie.setMaxAge(60 * 60);  // Cookie expires in 1 hour
                    resultCookie.setPath("/");  // Set the cookie path to the root of the application
                    response.addCookie(resultCookie);  // Add the cookie to the response
                }
            } catch (NumberFormatException ex) {
                out.println("<p>Error: Both input system and output system must be valid integers.</p>");
            } catch (InvalidInputException ex) {
                Logger.getLogger(ConvertServlet.class.getName()).log(Level.SEVERE, "Conversion error", ex);
                out.println("<p>Error: Invalid input encountered. Please check your number and system bases.</p>");
            }

            out.println("</body>");
            out.println("</html>");
        }
    }
}
