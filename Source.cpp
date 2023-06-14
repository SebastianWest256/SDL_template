#include <SDL.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "MathFunctions.cpp"
#include "RenderFunctions.cpp"
#include "InputHandling.cpp"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const int FPS = 60;
const int frameDelay = 1000 / FPS;



void initialize_SDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {

        exit(1);
    }
}

SDL_Window* create_window() {
    SDL_Window* window = SDL_CreateWindow("MapRender", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {

        SDL_Quit();
        exit(1);
    }
    return window;
}

SDL_Surface* get_window_surface(SDL_Window* window) {
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
    return screen;
}

input handle_events(bool& running, input current_input) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (current_input.mouse_pressed == true) {
            current_input.mouse_x = event.button.x;
            current_input.mouse_y = event.button.y;
        }
        if (event.type == SDL_QUIT) {
            running = false;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                current_input.mouse_pressed = true;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                current_input.mouse_pressed = false;
            }
        }
        else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            SDL_Keycode keyCode = event.key.keysym.sym;
            auto it = key_map.find(keyCode);
            if (it != key_map.end()) {
                int index = it->second;
                current_input.key_pressed[index] = (event.type == SDL_KEYDOWN);
                if (SDL_KEYUP) {
                    current_input.key_reset[index] = true;
                }
            }
        }
        
    }
    return current_input;
}

void clean_up(SDL_Window* window) {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {

    srand(time(0));

    Uint32 frameStart;
    int frameTime;

    input current_input;

    initialize_SDL();

    SDL_Window* window = create_window();

    SDL_Surface* screen = get_window_surface(window);

    //init start

    std::vector<textbox> textbox(std::vector<textbox>(2));
    textbox[0].init(400, 400, 4, 11, 0xBBBBBB, 0x777777, 0x444444);
    textbox[1].init(400, 200, 7, 5, 0xBBBBBB, 0x777777, 0x444444);
    int active_textbox = -1;
    bool using_textbox = false;
    //init end

    SDL_UpdateWindowSurface(window);

    bool running = true;
    while (running) {

        frameStart = SDL_GetTicks();

        //loop start

        draw_rectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xDD9955);

        handle_textboxes(screen, &textbox, &active_textbox, &using_textbox, &current_input);

        //loop end

        SDL_UpdateWindowSurface(window);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

        current_input = handle_events(running, current_input);
    }

    clean_up(window);

    return 0;
}
