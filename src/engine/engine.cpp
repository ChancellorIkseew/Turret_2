#include "engine.hpp"
//
#include "gui.hpp"
#include "game/mob/mob_type.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/sprite.hpp"
#include "engine/render/text.hpp"
#include "game/world/camera.hpp"
#include "game/world/map_drawer.hpp"
#include "game/weather/weather.hpp"

void Engine::run() {
    Atlas::addTexture("res/fonts/vc_latin.png");
    Atlas::addTexture("res/fonts/vc_cyrilic.png");
    //
    //Atlas::addTexture("res/images/icon.bmp");
    Atlas::addTexture("res/images/ice.png");
    //Atlas::addTexture("res/images/grass.png");
    Atlas::addTexture("res/images/magma.png");
    Atlas::addTexture("res/images/plate.png");
    Atlas::addTexture("res/images/rock.png");
    Atlas::addTexture("res/images/snow.png");
    Atlas::addTexture("res/images/soil.png");
    Atlas::addTexture("res/images/stone.png");
    Atlas::addTexture("res/images/water.png");
    //
    Atlas::addTexture("res/images/round_shadow.png");
    Atlas::addTexture("res/images/plant.png");
    Atlas::addTexture("res/images/cannoner_bot.png");
    Atlas::addTexture("res/images/fill.png");
    Atlas::addTexture("res/images/snowflake_1.png");
    Atlas::build();
    text::setFont("vc_latin", "vc_cyrilic");

    Mob mob(CANNON_BOSS);
    mob.setPixelCoord(PixelCoord(32, 32));
    mob.setAngleDeg(45.0f);

    GUI gui(mainWindow);
    TileCoord mapSize(200, 200);
    world = std::make_unique<World>(mapSize);
    Camera camera(mapSize);
    MapDrawer mapDrawer(camera, world->getMap());

    SDL_SetTextureBlendMode(Atlas::rawSDL(), SDL_BLENDMODE_BLEND);

    Weather weather;
    weather.init();
    weather.addFlake();
    
    while (mainWindow.isOpen()) {
        mainWindow.pollEvents();
        camera.interact(mainWindow.getSize());
        mainWindow.clear();
        mainWindow.setRenderScale(camera.getMapScale());
        mainWindow.setRenderTranslation(camera.getPosition());
        mapDrawer.draw();
        
        //SDL_SetTextureBlendMode(Atlas::rawSDL(), SDL_BLENDMODE_BLEND);
        mob.draw();
        mainWindow.setRenderScale(1.0f);
        mainWindow.setRenderTranslation(PixelCoord(0.0f, 0.0f));
        weather.draw();
        gui.draw(mainWindow);
        gui.acceptHotkeys(mainWindow);
        mainWindow.render();
    }
    Atlas::clear();
}
