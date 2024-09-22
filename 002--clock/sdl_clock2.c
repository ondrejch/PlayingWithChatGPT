#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define WIDTH 400
#define HEIGHT 400
#define RADIUS 150
#define CENTER_X (WIDTH / 2)
#define CENTER_Y (HEIGHT / 2)
#define SEC_HAND_LENGTH (RADIUS - 20)
#define MIN_HAND_LENGTH (RADIUS - 40)
#define HOUR_HAND_LENGTH (RADIUS - 70)

SDL_Color colors[] = {
    {255, 0, 0, 255},    // Red (for seconds hand)
    {255, 255, 255, 255} // White (for minute and hour hands)
};

void draw_circle(SDL_Renderer *renderer, int x, int y, int radius) {
    int dx, dy;
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            dx = radius - w;
            dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void draw_hand(SDL_Renderer *renderer, int angle, int length, SDL_Color color, int thickness) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    int x = CENTER_X + length * sin(angle * M_PI / 180);
    int y = CENTER_Y - length * cos(angle * M_PI / 180);
    
    for (int i = -thickness / 2; i <= thickness / 2; i++) {
        SDL_RenderDrawLine(renderer, CENTER_X + i, CENTER_Y + i, x, y);
    }
}

void render_text(SDL_Renderer *renderer, SDL_Color color, const char *text, int x, int y) {
    // Placeholder for rendering text (SDL_ttf can be used for this purpose)
    // Since we're keeping this code plain SDL2 without TTF, 
    // this part can be implemented using SDL_ttf to draw text.
}

int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Analog Clock", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int quit = 0;
    SDL_Event event;
    int color_index = 0;
    time_t rawtime;
    struct tm *timeinfo;
    Uint32 last_time = SDL_GetTicks();

    // Main loop
    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Get the current time
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        int seconds = timeinfo->tm_sec;
        int minutes = timeinfo->tm_min;
        int hours = timeinfo->tm_hour % 12;

        // Calculate angles for the hands
        int second_angle = seconds * 6;                    // 6 degrees per second (360 / 60)
        int minute_angle = minutes * 6 + seconds / 10;     // 6 degrees per minute, gradual movement per second
        int hour_angle = hours * 30 + minutes / 2;         // 30 degrees per hour, gradual movement per minute

        // Draw the clock's circle
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        draw_circle(renderer, CENTER_X, CENTER_Y, RADIUS);

        // Draw hour hand (white, shorter, thicker)
        draw_hand(renderer, hour_angle, HOUR_HAND_LENGTH, colors[1], 6);

        // Draw minute hand (white, longer, thinner)
        draw_hand(renderer, minute_angle, MIN_HAND_LENGTH, colors[1], 4);

        // Draw second hand with color changing every 0.1 second
        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_time > 100) {
            color_index = (color_index + 1) % 7;
            last_time = current_time;
        }
        draw_hand(renderer, second_angle, SEC_HAND_LENGTH, colors[0], 2);

        // Render the digital time display
        char time_str[9];
        sprintf(time_str, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        // This would be rendered using SDL_ttf in practice
        // Uncomment below line once SDL_ttf is used
        // render_text(renderer, colors[1], time_str, CENTER_X - 40, HEIGHT - 30);

        // Present the renderer
        SDL_RenderPresent(renderer);

        // Delay for 16ms (~60 FPS)
        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

