#include "engine.hpp"
//
#include "gameplay_gui.hpp"
#include "editor_gui.hpp"
#include "menu_gui.hpp"
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
    Atlas::addTexture("res/images/ice_block.png");
    Atlas::addTexture("res/images/round_shadow.png");
    Atlas::addTexture("res/images/plant.png");
    Atlas::addTexture("res/images/cannoner_bot.png");
    Atlas::addTexture("res/images/fill.png");
    Atlas::addTexture("res/images/snowflake_1.png");
    Atlas::build();
    text::setFont("vc_latin", "vc_cyrilic");

    while (mainWindow.isOpen()) {
        switch (state) {
        case EngineState::main_menu:  menu();     break;
        case EngineState::gameplay:   gameplay(); break;
        case EngineState::map_editor: editor();   break;
        case EngineState::exit:       mainWindow.close();
        }
    }
    Atlas::clear();
}

void Engine::gameplay() {
    Mob mob(CANNON_BOSS);
    mob.setPixelCoord(PixelCoord(32, 32));
    mob.setAngleDeg(45.0f);

    GameplayGUI gui(mainWindow, state);
    TileCoord mapSize(200, 200);
    std::unique_ptr<World> world;
    world = std::make_unique<World>(mapSize);
    Camera camera(mapSize);
    MapDrawer mapDrawer(camera, world->getMap());

    Weather weather;
    weather.init();
    weather.addFlake();

    while (mainWindow.isOpen() && state == EngineState::gameplay) {
        mainWindow.pollEvents();
        camera.interact(mainWindow.getSize());
        mainWindow.clear();
        mainWindow.setRenderScale(camera.getMapScale());
        mainWindow.setRenderTranslation(camera.getPosition());
        mapDrawer.draw();

        mob.draw();
        mainWindow.setRenderScale(1.0f);
        mainWindow.setRenderTranslation(PixelCoord(0.0f, 0.0f));
        weather.draw();
        gui.draw(mainWindow);
        gui.acceptHotkeys(mainWindow);
        mainWindow.render();
    }
}

#include "game/events/events.hpp"

void Engine::editor() {
    TileType tileType = TileType::SNOW;
    EditorGUI gui(mainWindow, state, tileType);
    TileCoord mapSize(200, 200);
    std::unique_ptr<World> world;
    world = std::make_unique<World>(mapSize);
    Camera camera(mapSize);
    MapDrawer mapDrawer(camera, world->getMap());

    while (mainWindow.isOpen() && state == EngineState::map_editor) {
        mainWindow.pollEvents();
        camera.interact(mainWindow.getSize());
        mainWindow.clear();
        mainWindow.setRenderScale(camera.getMapScale());
        mainWindow.setRenderTranslation(camera.getPosition());
        mapDrawer.draw();
        Events::clear();

        mainWindow.setRenderScale(1.0f);
        mainWindow.setRenderTranslation(PixelCoord(0.0f, 0.0f));
        gui.acceptHotkeys(mainWindow);
        gui.editMap(*world, camera, tileType);
        gui.draw(mainWindow);
        mainWindow.render();
    }
}

void Engine::menu() {
    MenuGUI gui(mainWindow, state);
    TileCoord mapSize(200, 200);
    std::unique_ptr<World> world;
    world = std::make_unique<World>(mapSize);
    Camera camera(mapSize);
    MapDrawer mapDrawer(camera, world->getMap());

    while (mainWindow.isOpen() && state == EngineState::main_menu) {
        mainWindow.pollEvents();
        camera.interact(mainWindow.getSize());
        mainWindow.clear();
        mainWindow.setRenderScale(camera.getMapScale());
        mainWindow.setRenderTranslation(camera.getPosition());
        mapDrawer.draw();

        mainWindow.setRenderScale(1.0f);
        mainWindow.setRenderTranslation(PixelCoord(0.0f, 0.0f));
        gui.draw(mainWindow);
        gui.acceptHotkeys(mainWindow);
        mainWindow.render();
    }
}