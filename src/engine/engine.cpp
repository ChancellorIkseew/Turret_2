#include "engine.hpp"
//
#include "gameplay_gui.hpp"
#include "editor_gui.hpp"
#include "menu_gui.hpp"
#include "game/mob/mob_type.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/sprite.hpp"
#include "engine/render/text.hpp"
#include "engine/parser/list_parser.hpp"
#include "game/world/camera.hpp"
#include "game/world/map_drawer.hpp"
#include "game/weather/weather.hpp"

static std::filesystem::path images("res/images");

void Engine::run() {
    list::Data textureNames = list::read("res/textures.list").value();
    for (const auto& name : textureNames) {
        Atlas::addTexture(images / name);
    }

    Atlas::addTexture("res/fonts/vc_latin.png");
    Atlas::addTexture("res/fonts/vc_cyrilic.png");
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