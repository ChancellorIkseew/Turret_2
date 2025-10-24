#include "engine.hpp"
//
#include <functional>
#include <mutex>
#include <thread>
#include "engine/gui/editor_gui.hpp"
#include "engine/gui/gameplay_gui.hpp"
#include "engine/gui/menu_gui.hpp"
//
#include "game/content/content.hpp"
#include "engine/render/atlas.hpp"
#include "engine/util/sleep.hpp"
//
#include "game/events/events.hpp"
#include "game/script_libs/lib_world_init.hpp"
#include "game/script_libs/script_libs.hpp"
#include "game/player/camera.hpp"
#include "game/world_drawer/world_drawer.hpp"
#include "game/presets/mob_presets.hpp"

#include "game/player/player_controller.hpp"
#include "game/world_saver/world_saver.hpp"
#include "game/generation/generation.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/settings/settings.hpp"
#include "game/world_saver/gen_preset_saver.hpp"
//
#include "game/physics/mob_ai.hpp"
#include "game/physics/turret_types.hpp"

void Engine::run() {
    script_libs::registerScripts(scriptsHandler);
    scriptsHandler.load();
    content::load();
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
    paused = Settings::gameplay.pauseOnWorldOpen;
    std::mutex worldMutex;
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
    player->getMobs().begin()->shootingAI = std::make_unique<PlayerControlledShooting>();
    player->getMobs().begin()->turret = std::make_unique<CannonTurret>(CTPreset);

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
    std::thread simulation([&] { startSimulation(*world, worldMutex); });
    //TODO: std::thread network([&] { startNet(); }); 
    while (mainWindow.isOpen() && worldOpen) {
        mainWindow.pollEvents();
        mainWindow.clear();
        {
            std::lock_guard<std::mutex> guard(worldMutex);
            camera.update(mainWindow.getSize());
            mainWindow.setRenderScale(camera.getMapScale());
            mainWindow.setRenderTranslation(camera.getPosition());
            auto deltaT = getDelta();
            PlayerController::update(*player, camera, *gui, deltaT);
            worldDrawer.draw(deltaT);
            Events::reset(); // for editor
        }
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

void Engine::startSimulation(World& world, std::mutex& worldMutex) {
    while (mainWindow.isOpen() && worldOpen) {
        if (!paused) {
            std::lock_guard<std::mutex> guard(worldMutex);
            simStart = mainWindow.getTime();
            for (auto& [teamID, team] : world.getTeams()) {
                team->interact(world);
            }
        }
        util::sleep(48);
    }
}

void Engine::startNet() {
    while (mainWindow.isOpen() && worldOpen) {
        util::sleep(48);
    }
}
