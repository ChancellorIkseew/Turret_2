#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
//
#include "render/sprite.hpp"
#include "window/window.hpp"
#include "window/input/input_handler.h"

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
    SDL_Event event;
    bool quit = false;

    Texture texture("res/images/icon.bmp");
    Sprite sprite(texture);
    sprite.setPosition(000, 0);
    Sprite sprite2(texture);
    sprite2.setPosition(100, 0);
    sprite2.setOrigin(50.0f, 50.0f);
    sprite2.setRotation(70);

    while (!quit) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                quit = true;
            InputHandler::updateInput(event);
        }

        if (InputHandler::active(t1::BindName::LMB))
            std::cout << "mouse\n";

        if (InputHandler::active(t1::BindName::Move_up))
            std::cout << "WWW\n";

        mainWindow.clear();
        sprite.drawFast();
        sprite2.draw();
        mainWindow.render();
    }
}