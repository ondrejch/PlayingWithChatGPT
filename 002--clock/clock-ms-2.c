#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

int main() {
    struct timeval tv;
    long last_millis = -1;

    while (1) {
        // Get the current time
        gettimeofday(&tv, NULL);
        
        // Calculate current milliseconds
        long current_millis = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
        long current_tenth = current_millis / 100;  // Divide by 100 to get tenths of a second

        // Check if the tenths of a second have changed
        if (current_tenth != last_millis / 100) {
            last_millis = current_millis;

            // Convert to a time structure
            struct tm *tm_info = localtime(&tv.tv_sec);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

            // Print date and full time including milliseconds
            printf("Current date and time: %s.%03ld\n", buffer, tv.tv_usec / 1000);
        }

        // Sleep for a short duration to avoid busy waiting
        usleep(50); // sleep for 50 microseconds
    }

    return 0;
}

