#include "engine.hpp"
//
#include <functional>
#include <thread>
#include "engine/gui/editor_gui.hpp"
#include "engine/gui/gameplay_gui.hpp"
#include "engine/gui/menu_gui.hpp"
//
#include "engine/content/load_content.hpp"
#include "engine/render/atlas.hpp"
#include "engine/util/sleep.hpp"
//
#include "game/events/events.hpp"
#include "game/script_libs/lib_world_init.hpp"
#include "game/script_libs/script_libs.hpp"
#include "game/world/camera.hpp"
#include "game/world_drawer/world_drawer.hpp"
#include "game/presets/mob_presets.hpp"

#include "game/player/player_controller.hpp"
#include "game/world_saver/world_saver.hpp"
#include "game/generation/generation.hpp"
//
#include "engine/io/folders.hpp"
#include "game/world_saver/gen_preset_saver.hpp"

void Engine::run() {
    script_libs::registerScripts(scriptsHandler);
    scriptsHandler.load();
    content::loadTextures();
    openMainMenu();
    while (mainWindow.isOpen()) {
        createScene(worldFolder, worldProperties);
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
    const auto floorPresets = serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT);
    const auto overlayPresets = serializer::loadOverlayPreset(io::folders::GENERATION_DEFAULT);
    worldProperties = WorldProperties(TileCoord(100, 100), 0U, floorPresets, overlayPresets);
}

void Engine::createScene(const std::string& folder, WorldProperties& properties) {
    std::unique_ptr<GUI> gui;
    std::unique_ptr<World> world;

    if (command == EngineCommand::gameplay_load_world || command == EngineCommand::editor_load_world) {
        world = serializer::loadWorld(folder);
        if (!world)
            return openMainMenu();
    }   
    else
        world = gen::generateWorld(properties);

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
    std::thread simulation([&] { startSimulation(*world); });
    //TODO: std::thread network([&] { startNet(); }); 
    while (mainWindow.isOpen() && worldOpen) {
        mainWindow.pollEvents();
        camera.interact(mainWindow.getSize());
        mainWindow.clear();
        mainWindow.setRenderScale(camera.getMapScale());
        mainWindow.setRenderTranslation(camera.getPosition());
        worldDrawer.draw();
        Events::reset(); // for editor
        MobController::interact(*player, camera);

        mainWindow.setRenderScale(1.0f);
        mainWindow.setRenderTranslation(PixelCoord(0.0f, 0.0f));
        gui->draw();
        gui->callback();
        mainWindow.render();
        scriptsHandler.execute();   
    }
    simulation.join();
    //TODO: network.join();
}

void Engine::startSimulation(World& world) {
    while (mainWindow.isOpen() && worldOpen) {
        for (auto& [teamID, team] : world.getTeams()) {
            team->interact(world);
        }
        util::sleep(48);
    }
}

void Engine::startNet() {
    while (mainWindow.isOpen() && worldOpen) {
        util::sleep(48);
    }
}
