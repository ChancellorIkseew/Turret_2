#pragma once
#include <memory>
#include "engine_command.hpp"
#include "engine/assets/assets.hpp"
#include "engine/scripting/scripting.hpp"
#include "engine/window/window.hpp"
#include "game/generation/generation.hpp"

class GUI;
class GameSession;

class Engine {
    MainWindow mainWindow;
    Assets assets;
    ScriptsHandler scriptsHandler;
    WorldProperties worldProperties;
    std::string worldFolder;
    EngineCommand command = EngineCommand::main_menu;
    GameSession* _session = nullptr;
public:
    Engine(const std::string& windowTitle) : mainWindow(windowTitle) { }
    void run();
    void loadWorldInGame(const std::string& folder);
    void loadWorldInEditor(const std::string& folder);
    void createWorldInGame(WorldProperties& properties);
    void createWorldInEditor();
    void openMainMenu();
    //
    void closeGame() { closeSession(); mainWindow.close(); }
    GameSession& getSession() { return *_session; }
    //
    const MainWindow& getMainWindow() const { return mainWindow; }
    const Assets& getAssets() const { return assets; }
    MainWindow& getMainWindow() { return mainWindow; }
    Assets& getAssets() { return assets; };
    GUI& getGUI();
private:
    void startSession(const std::string& folder, WorldProperties& properties);
    void closeSession();
};
