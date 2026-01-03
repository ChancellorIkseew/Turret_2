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
#include "game/script_libs/script_libs.hpp"
#include "game/player/camera.hpp"
#include "game/world_drawer/world_drawer.hpp"

#include "game/player/player_controller.hpp"
#include "game/world_saver/world_saver.hpp"
#include "game/generation/generation.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/settings/settings.hpp"
#include "engine/widgets/form_editor/form_editor.hpp"
#include "game/world_saver/gen_preset_saver.hpp"
#include "game/content/presets.hpp"
#include "game/physics/ai_system.hpp"
#include "game/physics/mobs_system.hpp"
#include "game/physics/shells_system.hpp"
#include "game/physics/turrets_system.hpp"

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
    paused = Settings::gameplay.pauseOnWorldOpen;
    std::mutex worldMutex;
    
    std::unique_ptr<World> world = createWorld(command, folder, properties);
    if (!world)
        return openMainMenu();
    Camera camera(world->getMap().getSize());
    std::unique_ptr<GUI> gui = createGUI(command, *this, world->getMap(), camera);
    PlayerController playerController;
    WorldDrawer worldDrawer;

    _world = world.get();
    _gui = gui.get();
    _camera = &camera;
    _playerController = &playerController;
    script_libs::initNewGame(*this);

    worldOpen = true;
    //TODO: std::jthread network([&] { startNet(); }); 

    //sim
    Team* playerTeam = world->getTeams().addTeam(U"player");
    playerController.setPlayerTeam(playerTeam);
    auto& mobs = world->getMobs();
    auto& shells = world->getShells();

    auto& cannonerBot = content::Presets::getMobs().at("cannoner_bot"); // throws if no .tin preset files
    MotionData mData(MovingAI::basic, 0, PixelCoord(400, 1000));
    ShootingData sData(ShootingAI::basic, false, PixelCoord(0, 0));
    mobs.addMob(cannonerBot, PixelCoord(100, 100), 0.f, cannonerBot->maxHealth, playerTeam->getID(), mData, sData,
        cannonerBot->turret->reload, 0.f);
    //
    while (mainWindow.isOpen() && isWorldOpen()) {
        mainWindow.pollEvents();
        mainWindow.clear();
        playerController.update(*this, world->getMobs());
        camera.update(mainWindow.getSize());
        mainWindow.setRenderScale(camera.getMapScale());
        mainWindow.setRenderTranslation(camera.getPosition());
        //
        if (!isPaused()) {
            // world.getChunks().update(mobs.getSoa()); not needed now, waits for better time
            shells::processShells(shells.getSoa(), mobs.getSoa());
            mobs::processMobs(mobs.getSoa());
            ai::updateMovingAI(mobs.getSoa(), playerController);
            ai::updateShootingAI(mobs.getSoa(), playerController);
            turrets::processTurrets(mobs.getSoa(), shells);
            // Clean up only after all processing.
            shells::cleanupShells(shells);
            mobs::cleanupMobs(mobs);
        }
        //
        worldDrawer.draw(camera, *world);
        Events::reset(); // for editor
        scriptsHandler.execute();
        //
        mainWindow.setRenderScale(1.0f);
        mainWindow.setRenderTranslation(PixelCoord(0.0f, 0.0f));
        gui->draw();
        gui->callback();
        mainWindow.render();
    }
}

void Engine::startNet() {
    while (mainWindow.isOpen() && isWorldOpen()) {
        util::sleep(48);
    }
}
