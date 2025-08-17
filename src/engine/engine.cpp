#include "engine.hpp"
//
#include "engine/gui/editor_gui.hpp"
#include "engine/gui/gameplay_gui.hpp"
#include "engine/gui/menu_gui.hpp"
//
#include "engine/window/input/controls.hpp"
#include "engine/content/load_content.hpp"
#include "engine/render/atlas.hpp"
//
#include "game/events/events.hpp"
#include "game/script_libs/lib_world_init.hpp"
#include "game/script_libs/script_libs.hpp"
#include "game/world/camera.hpp"
#include "game/world_drawer/world_drawer.hpp"
#include "game/mob/mob_presets.hpp"

#include "game/player/player_controller.hpp"
#include "game/world_saver/world_saver.hpp"
#include "game/generation/generation.hpp"

void Engine::run() {
    script_libs::registerScripts(scriptsHandler);
    scriptsHandler.load();
    content::loadTextures();

    worldProperties = WorldProperties(TileCoord(200, 200), 0U);
    while (mainWindow.isOpen()) {
        switch (command) {
        case EngineCommand::exit: mainWindow.close(); break;
        default: createScene(worldFolder, worldProperties); break;
        }
    }
    Atlas::clear();
}

void Engine::loadWorldInGame(const std::string& folder) {
    closeWorld();
    command = EngineCommand::gameplay_load_world;
    worldFolder = folder;
}
void Engine::loadWorldInEditor(const std::string& folder) {
    closeWorld();
    command = EngineCommand::editor_load_world;
    worldFolder = folder;
}
void Engine::createWorldInGame(WorldProperties& properties) {
    closeWorld();
    command = EngineCommand::gameplay_new_world;
    worldProperties = properties;
}
void Engine::createWorldInEditor(WorldProperties& properties) {
    closeWorld();
    command = EngineCommand::editor_new_world;
    worldProperties = properties;
}
void Engine::openMainMenu() {
    closeWorld();
    command = EngineCommand::main_menu;
    //worldProperties = ;
}

void Engine::createScene(const std::string& folder, WorldProperties& properties) {
    std::unique_ptr<GUI> gui;
    //std::unique_ptr<World> world;
    auto world = std::make_unique<World>();

    if (command == EngineCommand::gameplay_load_world || command == EngineCommand::editor_load_world)
        world = WorldSaver::load("new_world");
    else
        gen::generate(world->getMap(), properties);

    Camera camera(world->getMap().getSize());
    WorldDrawer worldDrawer(camera, *world);
    lib_world::init(world.get());

    Team* player = world->getTeams().addTeam(U"player");
    player->spawnMob(cannonBoss, PixelCoord(64, 64), 0.0f);

    switch (command) {
    case EngineCommand::main_menu:
        gui = std::make_unique<MenuGUI>(*this);
        break;
    case EngineCommand::gameplay_new_world:
    case EngineCommand::gameplay_load_world:
        gui = std::make_unique<GameplayGUI>(*this);
        break;
    case EngineCommand::editor_new_world:
    case EngineCommand::editor_load_world:
        gui = std::make_unique<EditorGUI>(*this, world->getMap(), camera);
        break;
    }

    _world = world.get();
    _gui = gui.get();

    worldOpen = true;
    while (mainWindow.isOpen() && worldOpen) {
        mainWindow.pollEvents();
        camera.interact(mainWindow.getSize());
        mainWindow.clear();
        mainWindow.setRenderScale(camera.getMapScale());
        mainWindow.setRenderTranslation(camera.getPosition());
        worldDrawer.draw();
        Events::clear(); // for editor
        MobController::interact(*player, camera);

        for (auto& [teamID, team] : world->getTeams()) {
            team->interact(*world);
        }

        mainWindow.setRenderScale(1.0f);
        mainWindow.setRenderTranslation(PixelCoord(0.0f, 0.0f));
        gui->draw();
        gui->callback();
        mainWindow.render();
        scriptsHandler.execute();
    }
}
