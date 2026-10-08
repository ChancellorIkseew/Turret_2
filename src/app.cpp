#include "app.hpp"
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
#include "game/game_session.hpp"

// Constuctor and destructor in cpp are needed for forward declaraton "GameSession" class in hpp.
App::App(const std::string& windowTitle, const PixelCoord windowSize) :
    mainWindow(windowTitle, windowSize), scriptsHandler(std::make_unique<ScriptsHandler>()) { }
App::~App() = default;

static std::unique_ptr<World> createWorld(const SessionRequest& request, const Assets& assets) {
    if (request.command == appCommand::gameplay_load_world || request.command == appCommand::editor_load_world)
        return serializer::loadWorld(request.worldFolder);
    return gen::generateWorld(request.worldProperties, assets);
}

static std::unique_ptr<GUI> createGUI(const appCommand command, App& app) {
    switch (command) {
    case appCommand::main_menu:
        return std::make_unique<MenuGUI>(app);
    case appCommand::gameplay_new_world:
    case appCommand::gameplay_load_world:
        return std::make_unique<GameplayGUI>(app);
    case appCommand::editor_new_world:
    case appCommand::editor_load_world:
        return std::make_unique<EditorGUI>(app);
    }
    throw std::runtime_error("Failed to create GUI.");
}

static GameMode getGameMode(const appCommand command) {
    switch (command) {
    case appCommand::main_menu:           return GameMode::menu;
    case appCommand::gameplay_new_world:  return GameMode::survival;
    case appCommand::gameplay_load_world: return GameMode::survival;
    default:                                 return GameMode::editor;
    }
}

static std::unique_ptr<GameSession> createSession(const SessionRequest& request, App& app) {
    Assets& assets = app.getAssets();
    std::unique_ptr<World> world = createWorld(request, assets);
    if (!world)
        return nullptr;
    const appCommand command = request.command;
    const bool paused = command == appCommand::main_menu ? false : Settings::gameplay.pauseOnWorldOpen;
    return std::make_unique<GameSession>(std::move(world), createGUI(command, app), assets, paused, getGameMode(command));
}

void App::run() {
    script_libs::registerScripts(*scriptsHandler);
    scriptsHandler->load();
    assets.load(mainWindow.getRenderer());
    openMainMenu();
    while (mainWindow.isOpen()) {
        processSessionRequest();
        if (session)
            session->update(*this, assets.getPresets(), *scriptsHandler);
        //
        if (mainWindow.hasLostFocus() && Settings::audio.muteInBackground) {
            assets.getAudio().setMasterVolume(0.f);
            assets.getAudio().updateVolume();
        }
        if (mainWindow.hasGainedFocus()) {
            assets.getAudio().setMasterVolume(static_cast<float>(Settings::audio.master) / 100.f);
            assets.getAudio().updateVolume();
        }
    }
}

void App::changeSession(SessionRequest request) {
    sessionRequest = std::move(request);
    assets.getAudio().stopMusic();
}

void App::loadWorldInGame(const std::string& folder) {
    changeSession({.command = appCommand::gameplay_load_world, .worldFolder = folder });
}
void App::loadWorldInEditor(const std::string& folder) {
    changeSession({ .command = appCommand::editor_load_world, .worldFolder = folder });
}
void App::createWorldInGame(WorldProperties properties) {
    changeSession({ .command = appCommand::gameplay_new_world, .worldProperties = std::move(properties) });
}
void App::createWorldInEditor() {
    const auto floorPresets = serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT);
    const auto overlayPresets = serializer::loadOverlayPreset(io::folders::GENERATION_DEFAULT);
    WorldConfig config{ .mapSize = TileCoord(100, 100), .seed = 0, .wavesByTimer = true, };
    WorldProperties properties(config, floorPresets, overlayPresets);
    changeSession({ .command = appCommand::editor_new_world, .worldProperties = properties });
}
void App::openMainMenu() {
    const auto floorPresets = serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT);
    const auto overlayPresets = serializer::loadOverlayPreset(io::folders::GENERATION_DEFAULT);
    WorldConfig config{ .mapSize = TileCoord(100, 100), .seed = 0, .wavesByTimer = true, };
    WorldProperties properties(config, floorPresets, overlayPresets);
    changeSession({ .command = appCommand::main_menu, .worldProperties = properties });
}

GUI& App::getGUI() { return session->getGUI(); }

void App::processSessionRequest() {
    if (!sessionRequest)
        return;
    std::unique_ptr<GameSession> newSession = createSession(*sessionRequest, *this);
    sessionRequest.reset();
    if (newSession)
        session.reset(newSession.release());
    script_libs::initNewGame(*this);
}
