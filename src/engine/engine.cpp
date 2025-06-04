#include "engine.hpp"
//
#include "render/sprite.hpp"
#include "window/input/input.hpp"

void Engine::run() {
    Texture texture("res/images/icon.bmp");
    Sprite sprite(texture);
    sprite.setPosition(000, 0);
    Sprite sprite2(texture);
    sprite2.setPosition(100, 0);
    sprite2.setOrigin(50.0f, 50.0f);
    sprite2.setRotation(70);

    SDL_Event event;

	while (mainWindow.isOpen()) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                mainWindow.close();
            Input::update(event);
        }

        mainWindow.clear();
        sprite.drawFast();
        sprite2.draw();
        mainWindow.render();
	}
}