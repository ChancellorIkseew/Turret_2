#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
//
#include "window.hpp"



int main(int argc, char* argv[]) {
    MainWindow mainWindow;

    const int FPS = 60;
    const Uint32 frameDelay = 1000 / FPS;

    SDL_Event e;
    bool quit = false;

    // Define a rectangle
    SDL_FRect greenSquare{ 270, 190, 100, 100 };

    auto renderer = mainWindow.getRenderer();

    SDL_Surface* healthbar_sur = IMG_Load("res/images/icon.bmp");
    SDL_Texture* healthbar_tex = SDL_CreateTextureFromSurface(renderer, healthbar_sur);
    SDL_DestroySurface(healthbar_sur); // Free surface after creating texture

    SDL_FRect rect{ 100, 100, 200, 200 };


    while (!quit) {
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(mainWindow.getRenderer(), 0, 0, 0, 255); // Set render draw color to black
        SDL_RenderClear(mainWindow.getRenderer()); // Clear the renderer

        //SDL_SetRenderDrawColor(mainWindow.getRenderer(), 0, 255, 0, 255); // Set render draw color to green
        //SDL_RenderFillRect(mainWindow.getRenderer(), &greenSquare); // Render the rectangle

        SDL_RenderTexture(mainWindow.getRenderer(), healthbar_tex, nullptr, &rect);

        SDL_RenderPresent(mainWindow.getRenderer()); // Render the screen

        Uint32 frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    return 0;
}