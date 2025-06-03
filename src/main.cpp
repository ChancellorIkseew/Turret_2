#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
//
#include "window/window.hpp"
#include "window/sprite.hpp"

void openMainWindow(MainWindow& mainWindow);

int main(int argc, char* argv[]) {

    try {
        MainWindow mainWindow("Turret_2.0.0");
        openMainWindow(mainWindow);
    }
    catch(std::exception exception) {

    }

    return 0;
}

void openMainWindow(MainWindow& mainWindow) {
    const int FPS = 60;
    const Uint32 frameDelay = 1000 / FPS;

    SDL_Event e;
    bool quit = false;

    Texture texture("res/images/icon.bmp");
    Sprite sprite(texture);
    sprite.setPosition(000, 0);
    Sprite sprite2(texture);
    sprite2.setPosition(100, 0);
    sprite2.setOrigin(50.0f, 50.0f);
    sprite2.setRotation(70);

    while (!quit) {
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        mainWindow.clear();
        sprite.drawFast();
        sprite2.draw();
        mainWindow.render();

        Uint32 frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
}