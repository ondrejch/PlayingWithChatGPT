#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>

#define CLOCK_RADIUS 10

void draw_clock(int hour, int minute, int second, int hundredths) {
    clear();
    int center_y = CLOCK_RADIUS + 1;
    int center_x = CLOCK_RADIUS + 1;

    // Draw the clock face
    for (int angle = 0; angle < 360; angle += 30) {
        int x = center_x + (int)(CLOCK_RADIUS * cos(angle * M_PI / 180));
        int y = center_y - (int)(CLOCK_RADIUS * sin(angle * M_PI / 180));
        mvprintw(y, x, "%d", angle / 30 == 0 ? 12 : angle / 30); // Print hours
    }

    // Draw the second hand
    int second_angle = (second % 60) * 6 + (hundredths >= 5 ? 6 : 0); // 6 degrees per second
    int second_x = center_x + (int)(CLOCK_RADIUS * 0.8 * cos(second_angle * M_PI / 180));
    int second_y = center_y - (int)(CLOCK_RADIUS * 0.8 * sin(second_angle * M_PI / 180));
    mvprintw(second_y, second_x, "S");

    // Draw the minute hand
    int minute_angle = (minute % 60) * 6; // 6 degrees per minute
    int minute_x = center_x + (int)(CLOCK_RADIUS * 0.6 * cos(minute_angle * M_PI / 180));
    int minute_y = center_y - (int)(CLOCK_RADIUS * 0.6 * sin(minute_angle * M_PI / 180));
    mvprintw(minute_y, minute_x, "M");

    // Draw the hour hand
    int hour_angle = (hour % 12) * 30 + (minute / 2); // 30 degrees per hour + minute influence
    int hour_x = center_x + (int)(CLOCK_RADIUS * 0.4 * cos(hour_angle * M_PI / 180));
    int hour_y = center_y - (int)(CLOCK_RADIUS * 0.4 * sin(hour_angle * M_PI / 180));
    mvprintw(hour_y, hour_x, "H");

    // Draw the center
    mvprintw(center_y, center_x, "O"); // Center of the clock

    refresh();
}

int main() {
    initscr(); // Initialize ncurses
    noecho();  // Don't echo typed characters
    curs_set(0); // Hide the cursor

    struct timeval tv;
    long last_hundredths = -1;

    while (1) {
        // Get the current time
        gettimeofday(&tv, NULL);
        struct tm *tm_info = localtime(&tv.tv_sec);
        int hour = tm_info->tm_hour;
        int minute = tm_info->tm_min;
        int second = tm_info->tm_sec;
        int hundredths = (tv.tv_usec / 10000) % 10; // Get tenths of a second

        // Check if the hundredths have changed
        if (hundredths != last_hundredths) {
            last_hundredths = hundredths;
            draw_clock(hour, minute, second, hundredths);
        }

        // Sleep for a short duration to avoid busy waiting
        usleep(100); // sleep for 10 milliseconds
    }

    endwin(); // End ncurses mode
    return 0;
}

