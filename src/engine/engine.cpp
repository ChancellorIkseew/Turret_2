#include "engine.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/settings/settings.hpp"
//
#include "engine/gui/editor_gui.hpp"
#include "engine/gui/gameplay_gui.hpp"
#include "engine/gui/menu_gui.hpp"
//
#include "game/script_libs/script_libs.hpp"
#include "game/generation/generation.hpp"
#include "game/world_saver/gen_preset_saver.hpp"
#include "game/world_saver/world_saver.hpp"
#include "game_session.hpp"

static std::unique_ptr<World> createWorld(const EngineCommand command, const std::string& folder,
    WorldProperties& properties, const Assets& assets) {
    if (command == EngineCommand::gameplay_load_world || command == EngineCommand::editor_load_world)
        return serializer::loadWorld(folder);
    return gen::generateWorld(properties, assets.getIndexes());
}

static std::unique_ptr<GUI> createGUI(const EngineCommand command, Engine& engine) {
    switch (command) {
    case EngineCommand::main_menu:
        return std::make_unique<MenuGUI>(engine);
    case EngineCommand::gameplay_new_world:
    case EngineCommand::gameplay_load_world:
        return std::make_unique<GameplayGUI>(engine);
    case EngineCommand::editor_new_world:
    case EngineCommand::editor_load_world:
        return std::make_unique<EditorGUI>(engine);
    }
    throw std::runtime_error("Failed to create GUI.");
}

void Engine::run() {
    script_libs::registerScripts(scriptsHandler);
    scriptsHandler.load();
    assets.load(mainWindow.getRenderer());
    openMainMenu();
    while (mainWindow.isOpen()) {
        startSession(worldFolder, worldProperties);
    }
}

void Engine::loadWorldInGame(const std::string& folder) {
    closeSession();
    command = EngineCommand::gameplay_load_world;
    worldFolder = folder;
}
void Engine::loadWorldInEditor(const std::string& folder) {
    closeSession();
    command = EngineCommand::editor_load_world;
    worldFolder = folder;
}
void Engine::createWorldInGame(WorldProperties& properties) {
    closeSession();
    command = EngineCommand::gameplay_new_world;
    worldProperties = properties;
}
void Engine::createWorldInEditor() {
    closeSession();
    command = EngineCommand::editor_new_world;
    const auto floorPresets = serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT);
    const auto overlayPresets = serializer::loadOverlayPreset(io::folders::GENERATION_DEFAULT);
    worldProperties = WorldProperties(TileCoord(100, 100), 0U, floorPresets, overlayPresets);
}
void Engine::openMainMenu() {
    closeSession();
    command = EngineCommand::main_menu;
    const auto floorPresets = serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT);
    const auto overlayPresets = serializer::loadOverlayPreset(io::folders::GENERATION_DEFAULT);
    worldProperties = WorldProperties(TileCoord(100, 100), 0U, floorPresets, overlayPresets);
}

void Engine::startSession(const std::string& folder, WorldProperties& properties) {
    std::unique_ptr<World> world = createWorld(command, folder, properties, assets);
    if (!world) {
        openMainMenu();
        return;
    }

    const bool paused = Settings::gameplay.pauseOnWorldOpen;
    GameSession session(std::move(world), createGUI(command, *this), assets, paused);
    _session = &session;
    script_libs::initNewGame(*this);

    while (mainWindow.isOpen() && session.isOpen()) {
        session.update(*this, assets.getPresets(), scriptsHandler);
    }
}

void Engine::closeSession() { if (_session) _session->close(); }
GUI& Engine::getGUI() { return _session->getGUI(); }

