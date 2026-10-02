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
Engine::Engine(const std::string& windowTitle, const PixelCoord windowSize) :
    mainWindow(windowTitle, windowSize), scriptsHandler(std::make_unique<ScriptsHandler>()) { }
Engine::~Engine() = default;

static std::unique_ptr<World> createWorld(const SessionRequest& request, const Assets& assets) {
    if (request.command == EngineCommand::gameplay_load_world || request.command == EngineCommand::editor_load_world)
        return serializer::loadWorld(request.worldFolder);
    return gen::generateWorld(request.worldProperties, assets);
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

static GameMode getGameMode(const EngineCommand command) {
    switch (command) {
    case EngineCommand::main_menu:           return GameMode::menu;
    case EngineCommand::gameplay_new_world:  return GameMode::survival;
    case EngineCommand::gameplay_load_world: return GameMode::survival;
    default:                                 return GameMode::editor;
    }
}

static std::unique_ptr<GameSession> createSession(const SessionRequest& request, Engine& engine) {
    Assets& assets = engine.getAssets();
    std::unique_ptr<World> world = createWorld(request, assets);
    if (!world)
        return nullptr;
    const EngineCommand command = request.command;
    const bool paused = command == EngineCommand::main_menu ? false : Settings::gameplay.pauseOnWorldOpen;
    return std::make_unique<GameSession>(std::move(world), createGUI(command, engine), assets, paused, getGameMode(command));
}

void Engine::run() {
    script_libs::registerScripts(*scriptsHandler);
    scriptsHandler->load();
    assets.load(mainWindow.getRenderer());
    openMainMenu();
    while (mainWindow.isOpen()) {
        processSessionRequest();
        if (session)
            session->update(*this, assets.getPresets(), *scriptsHandler);
    }
}

void Engine::changeSession(SessionRequest request) {
    sessionRequest = std::move(request);
}

void Engine::loadWorldInGame(const std::string& folder) {
    changeSession({.command = EngineCommand::gameplay_load_world, .worldFolder = folder });
}
void Engine::loadWorldInEditor(const std::string& folder) {
    changeSession({ .command = EngineCommand::editor_load_world, .worldFolder = folder });
}
void Engine::createWorldInGame(WorldProperties properties) {
    changeSession({ .command = EngineCommand::gameplay_new_world, .worldProperties = std::move(properties) });
}
void Engine::createWorldInEditor() {
    const auto floorPresets = serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT);
    const auto overlayPresets = serializer::loadOverlayPreset(io::folders::GENERATION_DEFAULT);
    WorldConfig config{ .mapSize = TileCoord(100, 100), .seed = 0, .toggleWaves = false, };
    WorldProperties properties(config, floorPresets, overlayPresets);
    changeSession({ .command = EngineCommand::editor_new_world, .worldProperties = properties });
}
void Engine::openMainMenu() {
    const auto floorPresets = serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT);
    const auto overlayPresets = serializer::loadOverlayPreset(io::folders::GENERATION_DEFAULT);
    WorldConfig config{ .mapSize = TileCoord(100, 100), .seed = 0, .toggleWaves = false, };
    WorldProperties properties(config, floorPresets, overlayPresets);
    changeSession({ .command = EngineCommand::main_menu, .worldProperties = properties });
}

GUI& Engine::getGUI() { return session->getGUI(); }

void Engine::processSessionRequest() {
    if (!sessionRequest)
        return;
    std::unique_ptr<GameSession> newSession = createSession(*sessionRequest, *this);
    sessionRequest.reset();
    if (newSession)
        session.reset(newSession.release());
    script_libs::initNewGame(*this);
}
