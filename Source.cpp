#include <SDL.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <vector>
#include "MathFunctions.cpp"
#include "RenderFunctions.cpp"

const int screenWidth = 1000;
const int screenHeight = 800;
const int FPS = 60;
const int frameDelay = 1000 / FPS;

void InitializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Handle error
        exit(1);
    }
}

SDL_Window* CreateWindow() {
    SDL_Window* window = SDL_CreateWindow("MapRender", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        // Handle error
        SDL_Quit();
        exit(1);
    }
    return window;
}

SDL_Surface* GetWindowSurface(SDL_Window* window) {
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
    return screen;
}

void HandleEvents(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
}

void CleanUp(SDL_Window* window) {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {

    srand(time(0));

    Uint32 frameStart;
    int frameTime;

    InitializeSDL();

    SDL_Window* window = CreateWindow();

    SDL_Surface* screen = GetWindowSurface(window);

    //init start

    drawSquare(screen, 100, 100, 50, 0xFF0000);

    //init end

    SDL_UpdateWindowSurface(window);

    bool running = true;
    while (running) {

        frameStart = SDL_GetTicks();

        //loop start

        //loop end

        SDL_UpdateWindowSurface(window);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

        HandleEvents(running);
    }

    CleanUp(window);

    return 0;
}