#pragma once
#include <memory>
#include "engine_command.hpp"
#include "engine/assets/assets.hpp"
#include "engine/scripting/scripting.hpp"
#include "engine/window/window.hpp"
#include "game/generation/generation.hpp"

class GameSession;
class GUI;

class Engine {
    MainWindow mainWindow;
    Assets assets;
    ScriptsHandler scriptsHandler;
    WorldProperties worldProperties;
    std::string worldFolder;
    EngineCommand command = EngineCommand::main_menu;
    std::unique_ptr<GameSession> session;
public:
    Engine(const std::string& windowTitle);
    ~Engine();
    void run();
    void loadWorldInGame(const std::string& folder);
    void loadWorldInEditor(const std::string& folder);
    void createWorldInGame(WorldProperties& properties);
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
    std::unique_ptr<GameSession> createSession();
    void closeSession();
    t1_disable_copy_and_move(Engine)
};
