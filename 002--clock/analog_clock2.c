#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define WIDTH 400
#define HEIGHT 400
#define RADIUS 150
#define CENTER_X (WIDTH / 2)
#define CENTER_Y (HEIGHT / 2)
#define SEC_HAND_LENGTH (RADIUS - 20)

// Colors for seconds hand
unsigned long colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF, 0xFFFFFF};

void draw_hand(Display *d, Window w, GC gc, int angle, int length, unsigned long color) {
    int x = CENTER_X + length * sin(angle * M_PI / 180);
    int y = CENTER_Y - length * cos(angle * M_PI / 180);
    XSetForeground(d, gc, color);
    XDrawLine(d, w, gc, CENTER_X, CENTER_Y, x, y);
}

void draw_circle(Display *d, Window w, GC gc) {
    XDrawArc(d, w, gc, CENTER_X - RADIUS, CENTER_Y - RADIUS, RADIUS * 2, RADIUS * 2, 0, 360 * 64);
}

int main() {
    Display *d;
    Window w;
    XEvent e;
    int s;
    
    // Initialize display and create a window
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, WIDTH, HEIGHT, 1, BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);

    GC gc = XCreateGC(d, w, 0, 0);
    XSetForeground(d, gc, BlackPixel(d, s));

    time_t rawtime;
    struct tm *timeinfo;
    
    int color_index = 0;

    // Main loop
    while (1) {
        XNextEvent(d, &e);

        if (e.type == Expose) {
            while (1) {
                // Get current time
                time(&rawtime);
                timeinfo = localtime(&rawtime);

                int seconds = timeinfo->tm_sec;
                int milliseconds = timeinfo->tm_sec * 1000 + timeinfo->tm_sec / CLOCKS_PER_SEC;
                
                // Clear window
                XClearWindow(d, w);

                // Draw the clock's circle
                draw_circle(d, w, gc);

                // Draw second hand
                int second_angle = seconds * 6;  // 360 degrees / 60 seconds = 6 degrees per second
                draw_hand(d, w, gc, second_angle, SEC_HAND_LENGTH, colors[color_index]);

                // Update color every 0.1 seconds
                color_index = (color_index + 1) % 7;

                // Sleep for 100ms
                usleep(100000);
            }
        }

        if (e.type == KeyPress)
            break;
    }

    // Cleanup
    XFreeGC(d, gc);
    XDestroyWindow(d, w);
    XCloseDisplay(d);

    return 0;
}

