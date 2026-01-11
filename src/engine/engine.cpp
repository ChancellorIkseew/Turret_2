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

// Constuctor and destructor in cpp are needed for forward declaraton "GameSession" class in hpp.
Engine::Engine(const std::string& windowTitle) : mainWindow(windowTitle) { }
Engine::~Engine() = default;

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

std::unique_ptr<GameSession> Engine::createSession() {
    std::unique_ptr<World> world = createWorld(command, worldFolder, worldProperties, assets);
    if (!world) {
        openMainMenu();
        return nullptr;
    }
    const bool paused = command == EngineCommand::main_menu ? false : Settings::gameplay.pauseOnWorldOpen;
    return std::make_unique<GameSession>(std::move(world), createGUI(command, *this), assets, paused);
}

void Engine::run() {
    script_libs::registerScripts(scriptsHandler);
    scriptsHandler.load();
    assets.load(mainWindow.getRenderer());
    openMainMenu();
    while (mainWindow.isOpen()) {
        if (session && session->isOpen())
            session->update(*this, assets.getPresets(), scriptsHandler);
        else {
            session = createSession();
            script_libs::initNewGame(*this);
        }
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

GUI& Engine::getGUI() { return session->getGUI(); }

void Engine::closeSession() {
    if (session) session->close();
}
