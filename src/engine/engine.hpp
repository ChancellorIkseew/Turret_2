#pragma once
#include <stdexcept>
#include "engine_command.hpp"
#include "engine/scripting/scripting.hpp"
#include "engine/window/window.hpp"
#include "game/generation/generation.hpp"

class World;
class GUI;

class Engine {
    MainWindow mainWindow;
    ScriptsHandler scriptsHandler;
    WorldProperties worldProperties;
    std::string worldFolder;
    World* _world = nullptr;
    GUI* _gui = nullptr;
    EngineCommand command = EngineCommand::main_menu;
    bool worldOpen = false;
public:
    Engine(const std::string& windowTitle) : mainWindow(windowTitle) { }
    void run();
    void createScene(const std::string& folder, WorldProperties& properties);
    void loadWorldInGame(const std::string& folder);
    void loadWorldInEditor(const std::string& folder);
    void createWorldInGame(WorldProperties& properties);
    void createWorldInEditor(WorldProperties& properties);
    void openMainMenu();
    void closeGame() { command = EngineCommand::exit; }
    void closeWorld() { worldOpen = false; }
    //
    MainWindow& getMainWindow() { return mainWindow; }
    World& getWorld() { return *_world; }
    GUI& getGUI() { return *_gui; }
};
