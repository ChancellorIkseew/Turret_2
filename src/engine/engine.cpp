#include "engine.hpp"
//
#include "gui.hpp"
#include "game/mob/mob_type.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/sprite.hpp"
#include "engine/window/input/input.hpp"
#include "game/world/camera.hpp"

void Engine::run() {
    Atlas::addTexture("res/fonts/font_0.png");
    Atlas::addTexture("res/images/icon.bmp");
    Atlas::addTexture("res/images/cannoner_bot.png");
    Atlas::addTexture("res/images/fill.png");
    Atlas::build();

    world = std::make_unique<World>();
    Mob mob(CANNON_BOSS);
    mob.setPixelCoord(PixelCoord(32, 32));
    mob.setAngleDeg(45.0f);

    GUI gui(mainWindow);
    TileCoord mapSize(100, 100);
    Camera camera(mapSize);
    
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
        mainWindow.render();
    }
    Atlas::clear();
}
