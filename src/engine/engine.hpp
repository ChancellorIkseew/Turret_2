#pragma once
#include <memory>
#include "engine_command.hpp"
#include "engine/assets/assets.hpp"
#include "engine/window/window.hpp"
#include "game/generation/generation.hpp"

class GameSession;
class GUI;
class ScriptsHandler;

struct SessionRequest {
    EngineCommand command{ EngineCommand::main_menu };
    std::string worldFolder;
    WorldProperties worldProperties;
};

class Engine {
    MainWindow mainWindow;
    Assets assets;
    std::optional<SessionRequest> sessionRequest;
    std::unique_ptr<GameSession> session;
    std::unique_ptr<ScriptsHandler> scriptsHandler;
public:
    Engine(const std::string& windowTitle, const PixelCoord windowSize);
    ~Engine();
    void run();
    void changeSession(SessionRequest request);
    void loadWorldInGame(const std::string& folder);
    void loadWorldInEditor(const std::string& folder);
    void createWorldInGame(WorldProperties properties);
    void createWorldInEditor();
    void openMainMenu();
    //
    void closeGame() { mainWindow.close(); }
    GameSession& getSession() { return *session; }
    //
    const MainWindow& getMainWindow() const { return mainWindow; }
    const Assets& getAssets() const { return assets; }
    MainWindow& getMainWindow() { return mainWindow; }
    Assets& getAssets() { return assets; };
    GUI& getGUI();
private:
    void processSessionRequest();
    t1_disable_copy_and_move(Engine)
};
