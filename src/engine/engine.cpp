#include "engine.hpp"
//
#include "gui.hpp"
#include "game/mob_type.hpp"
#include "render/atlas.hpp"
#include "render/sprite.hpp"
#include "window/input/input.hpp"
#include "world/camera.hpp"

void Engine::run() {
    Atlas::addTexture("res/images/icon.bmp");
    Atlas::addTexture("res/images/cannoner_bot.png");
    Atlas::addTexture("res/images/fill.png");
    Atlas::build();

    world = std::make_unique<World>();
    Sprite sprite("icon");
    sprite.setPosition(000, 0);
    Sprite sprite2("icon");
    sprite2.setPosition(100, 0);
    sprite2.setOrigin(50.0f, 50.0f);
    sprite2.setRotation(70);
    MobType standard;
    Mob mob(standard);
    mob.setPixelCoord(PixelCoord(32, 32));
    mob.setAngleDeg(45.0f);
 
    GUI gui(mainWindow);
    TileCoord mapSize(100, 100);
    Camera camera(mapSize);
    world->print();
    
    while (mainWindow.isOpen()) {
        mainWindow.pollEvents();
        camera.interact(mainWindow.getSize());
        mainWindow.clear();
        mainWindow.setRenderScale(camera.getMapScale());
        mainWindow.setRenderTranslation(camera.getPosition());
        world->draw(camera);
        mob.draw();
        mainWindow.setRenderScale(1.0f);
        mainWindow.setRenderTranslation(PixelCoord(0.0f, 0.0f));
        gui.draw(mainWindow);
        sprite.drawFast();
        mainWindow.render();
    }
    Atlas::clear();
}
