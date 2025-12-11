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
#include "engine/widgets/form_editor/form_editor.hpp"
#include "game/world_saver/gen_preset_saver.hpp"
//
#include "game/physics/mob_ai.hpp"
#include "game/physics/turret_types.hpp"

static std::unique_ptr<World> createWorld(const EngineCommand command, const std::string& folder, WorldProperties& properties) {
    if (command == EngineCommand::gameplay_load_world || command == EngineCommand::editor_load_world)
        return serializer::loadWorld(folder);
    return gen::generateWorld(properties);
}

static std::unique_ptr<GUI> createGUI(const EngineCommand command, Engine& engine, WorldMap& map, const Camera& camera) {
    switch (command) {
    case EngineCommand::main_menu:
        return std::make_unique<MenuGUI>(engine);
    case EngineCommand::gameplay_new_world:
    case EngineCommand::gameplay_load_world:
        return std::make_unique<GameplayGUI>(engine);
    case EngineCommand::editor_new_world:
    case EngineCommand::editor_load_world:
        return std::make_unique<EditorGUI>(engine, map, camera);
    }
    throw std::runtime_error("Failed to create GUI.");
}

void Engine::run() {
    script_libs::registerScripts(scriptsHandler);
    scriptsHandler.load();
    content::load();
    FormEditor::init();
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
void Engine::createWorldInEditor() {
    closeWorld();
    command = EngineCommand::editor_new_world;
    const auto floorPresets = serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT);
    const auto overlayPresets = serializer::loadOverlayPreset(io::folders::GENERATION_DEFAULT);
    worldProperties = WorldProperties(TileCoord(100, 100), 0U, floorPresets, overlayPresets);
}
void Engine::openMainMenu() {
    closeWorld();
    command = EngineCommand::main_menu;
    const auto floorPresets = serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT);
    const auto overlayPresets = serializer::loadOverlayPreset(io::folders::GENERATION_DEFAULT);
    worldProperties = WorldProperties(TileCoord(100, 100), 0U, floorPresets, overlayPresets);
}

void Engine::createScene(const std::string& folder, WorldProperties& properties) {
    int tickSpeed = 1;
    float tickTime = 48.0f;
    float tickOfset = 0.0f;
    paused = Settings::gameplay.pauseOnWorldOpen;
    std::mutex worldMutex;
    
    std::unique_ptr<World> world = createWorld(command, folder, properties);
    if (!world)
        return openMainMenu();
    Camera camera(world->getMap().getSize());
    WorldDrawer worldDrawer(camera, *world);
    std::unique_ptr<GUI> gui = createGUI(command, *this, world->getMap(), camera);
    lib_world::init(world.get());

    Team* player = world->getTeams().addTeam(U"player");
    player->spawnMob(cannonBoss, PixelCoord(64, 64), 0.0f);
    player->getMobs().begin()->shootingAI = std::make_unique<PlayerControlledShooting>();
    player->getMobs().begin()->turret = std::make_unique<CannonTurret>(CTPreset);

    _world = world.get();
    _gui = gui.get();

    worldOpen = true;
    std::thread simulation([&] { startSimulation(*world, worldMutex); });
    //TODO: std::thread network([&] { startNet(); }); 
    while (mainWindow.isOpen() && isWorldOpen()) {
        mainWindow.pollEvents();
        mainWindow.clear();
        camera.update(mainWindow.getSize());
        mainWindow.setRenderScale(camera.getMapScale());
        mainWindow.setRenderTranslation(camera.getPosition());
        {
            std::lock_guard<std::mutex> guard(worldMutex);
            if (isPaused())
                tickOfset = static_cast<float>(pauseStart - currentTickStart) / tickTime;
            else
                tickOfset = static_cast<float>(mainWindow.getTime() - currentTickStart) / tickTime;
            PlayerController::update(*player, camera, *gui, tickOfset);
            worldDrawer.draw(tickOfset);
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
    while (mainWindow.isOpen() && isWorldOpen()) {
        if (isPaused())
            util::sleep(1);
        else {
            {
                std::lock_guard<std::mutex> guard(worldMutex);
                for (auto& [teamID, team] : world.getTeams()) {
                    team->interact(world);
                }
                currentTickStart = mainWindow.getTime();
            }
            util::sleep(48);
        }
    }
}

void Engine::startNet() {
    while (mainWindow.isOpen() && isWorldOpen()) {
        util::sleep(48);
    }
}
