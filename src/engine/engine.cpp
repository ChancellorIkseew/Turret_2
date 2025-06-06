#include "engine.hpp"
//
#include "render/sprite.hpp"
#include "window/input/input.hpp"
#include "world/camera.hpp"

void Engine::run() {
    
    Texture texture("res/images/icon.bmp");
    Sprite sprite(texture);
    sprite.setPosition(000, 0);
    Sprite sprite2(texture);
    sprite2.setPosition(100, 0);
    sprite2.setOrigin(50.0f, 50.0f);
    sprite2.setRotation(70);
    
    TileCoord mapSize(100, 100);
    Camera camera(mapSize);
    world->print();

    while (mainWindow.isOpen()) {
        mainWindow.pollEvents();
        camera.interact(mainWindow.getSize());
        mainWindow.clear();
        mainWindow.setRenderScale(camera.getMapScale());
        world->draw(camera);
        mainWindow.setRenderScale(1.0f);
        //gui->draw(); todo: implement
        sprite.drawFast();
        mainWindow.render();
    }
}
