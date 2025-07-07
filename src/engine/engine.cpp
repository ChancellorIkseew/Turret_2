#include "engine.hpp"
//
#include "engine/gui/editor_gui.hpp"
#include "engine/gui/gameplay_gui.hpp"
#include "engine/gui/menu_gui.hpp"
//
#include "engine/content/load_content.hpp"
#include "engine/render/atlas.hpp"
//
#include "game/events/events.hpp"
#include "game/script_libs/script_libs.hpp"
#include "game/world/camera.hpp"
#include "game/world/map_drawer.hpp"

void Engine::run() {
    script_libs::registerScripts(scriptsHandler);
    scriptsHandler.load();

    content::loadTextures();
    while (mainWindow.isOpen()) {
        switch (state) {
        case EngineState::exit: mainWindow.close(); break;
        default:                createScene(state); break;
        }
    }
    Atlas::clear();
}

void Engine::createScene(const EngineState requiredState) {
    std::unique_ptr<GUI> gui;
    std::unique_ptr<World> world;
    
    TileCoord mapSize(200, 200);
    world = std::make_unique<World>(mapSize);
    Camera camera(mapSize);
    MapDrawer mapDrawer(camera, *world);

    switch (requiredState) {
    case EngineState::main_menu:
        gui = std::make_unique<MenuGUI>(mainWindow, state);
        break;
    case EngineState::gameplay:
        gui = std::make_unique<GameplayGUI>(mainWindow, state);
        break;
    case EngineState::map_editor:
        gui = std::make_unique<EditorGUI>(mainWindow, state, *world, camera);
        break;
    }

    while (mainWindow.isOpen() && state == requiredState) {
        mainWindow.pollEvents();
        camera.interact(mainWindow.getSize());
        mainWindow.clear();
        mainWindow.setRenderScale(camera.getMapScale());
        mainWindow.setRenderTranslation(camera.getPosition());
        mapDrawer.draw();
        Events::clear(); // for editor

        mainWindow.setRenderScale(1.0f);
        mainWindow.setRenderTranslation(PixelCoord(0.0f, 0.0f));
        gui->draw();
        gui->callback();
        mainWindow.render();
        scriptsHandler.execute();
    }
}
