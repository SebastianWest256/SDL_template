#include <SDL.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "MathFunctions.cpp"
#include "RenderFunctions.cpp"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const int FPS = 60;
const int frameDelay = 1000 / FPS;

std::unordered_map<SDL_Keycode, int> key_map = {
        {SDLK_a, 0}, {SDLK_b, 1}, {SDLK_c, 2}, {SDLK_d, 3}, {SDLK_e, 4}, {SDLK_f, 5}, {SDLK_g, 6}, {SDLK_h, 7}, {SDLK_i, 8}, {SDLK_j, 9},
        {SDLK_k, 10}, {SDLK_l, 11}, {SDLK_m, 12}, {SDLK_n, 13}, {SDLK_o, 14}, {SDLK_p, 15}, {SDLK_q, 16}, {SDLK_r, 17}, {SDLK_s, 18}, {SDLK_t, 19},
        {SDLK_u, 20}, {SDLK_v, 21}, {SDLK_w, 22}, {SDLK_x, 23}, {SDLK_y, 24}, {SDLK_z, 25}, {SDLK_0, 26}, {SDLK_1, 27}, {SDLK_2, 28}, {SDLK_3, 29}, 
        {SDLK_4, 30}, {SDLK_5, 31}, {SDLK_6, 32}, {SDLK_7, 33}, {SDLK_8, 34}, {SDLK_9, 35}, {SDLK_SPACE, 36}, {SDLK_PERIOD, 37}
};

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

    

    //init end

    SDL_UpdateWindowSurface(window);

    bool running = true;
    while (running) {

        frameStart = SDL_GetTicks();

        //loop start

        draw_rectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xDD9955);

        draw_char(screen, input_to_char(current_input), 100, 100, 10, 0x666666);

        if (char_to_int(input_to_char(current_input)) != -1) {
            if (current_input.key_reset[char_to_int(input_to_char(current_input))]) {
                std::cout << input_to_char(current_input) << std::endl;
                current_input.key_reset[char_to_int(input_to_char(current_input))] = false;
            }
        }

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
