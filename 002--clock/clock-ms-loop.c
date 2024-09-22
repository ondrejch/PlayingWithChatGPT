#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h> // For usleep

int main() {
    struct timeval tv;
    struct tm* tm_info;
    char buffer[26];

    while (1) {
        // Get the current time with microsecond precision
        gettimeofday(&tv, NULL);

        // Convert to local time
        tm_info = localtime(&tv.tv_sec);

        // Format the time to a string
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

        // Print the time with milliseconds
        printf("Current time: %s.%03ld\n", buffer, tv.tv_usec / 1000);

        // Sleep for 100 milliseconds (100000 microseconds)
        usleep(100000);
    }

    return 0;
}

