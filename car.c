#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    Display *display;
    Window window;
    XEvent event;
    int screen_width, screen_height;

    // Open connection to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        return 1; // Unable to open display
    }

    // Get screen dimensions
    screen_width = XDisplayWidth(display, DefaultScreen(display));
    screen_height = XDisplayHeight(display, DefaultScreen(display));

    // Seed the random number generator
    srand(time(NULL));

    // Generate random position for the window
    int window_width = 500;
    int window_height = 300;
    int x = rand() % (screen_width - window_width);
    int y = rand() % (screen_height - window_height);

    // Print random coordinates for debugging
    //printf("Random window position: x=%d, y=%d\n", x, y);

    // Create a simple window at the random position
    window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                                  x, y, window_width, window_height, 1,
                                  BlackPixel(display, 0),
                                  WhitePixel(display, 0));

    // Set window attributes to override window manager
    XSetWindowAttributes attrs;
    attrs.override_redirect = True; // Prevent the window manager from controlling this window
    XChangeWindowAttributes(display, window, CWOverrideRedirect, &attrs);

    // Set window title
    XStoreName(display, window, "Pop-up Title");

    // Select input events
    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask);

    // Map (show) the window
    XMapWindow(display, window);

    // The message to display
    char message[] = "We've been trying to reach you concerning your vehicle's extended warranty.\n"
                     "You should've received a notice in the mail about your car's extended warranty eligibility.\n"
                     "Since we've not gotten a response, we're giving you a final courtesy call before we close out your file.\n"
                     "Press 2 to be removed and placed on our do-not-call list.\n"
                     "To speak to someone about possibly extending or reinstating your vehicle's warranty, press 1 to speak with a warranty specialist.";

    // Event loop
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            // Draw text in the window
            int y_offset = 20;
            char *line = strtok(message, "\n");
            while (line != NULL) {
                XDrawString(display, window, DefaultGC(display, 0), 10, y_offset, line, strlen(line));
                line = strtok(NULL, "\n");
                y_offset += 20; // Move down for the next line
            }
        }
        if (event.type == ButtonPress) {
            break; // Exit on mouse click inside the window
        }
    }

    // Clean up and close the display
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}
