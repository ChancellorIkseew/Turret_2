#pragma once
#include <atomic>
#include "engine_command.hpp"
#include "engine/scripting/scripting.hpp"
#include "engine/window/window.hpp"
#include "game/generation/generation.hpp"

namespace std { class mutex; }
class World;
class GUI;

class Engine {
    uint64_t currentTickStart = 0U, pauseStart = 0U;
    MainWindow mainWindow;
    ScriptsHandler scriptsHandler;
    WorldProperties worldProperties;
    std::string worldFolder;
    World* _world = nullptr;
    GUI* _gui = nullptr;
    EngineCommand command = EngineCommand::main_menu;
    bool worldOpen = false, paused = false;
public:
    Engine(const std::string& windowTitle) : mainWindow(windowTitle) { }
    void run();
    void createScene(const std::string& folder, WorldProperties& properties);
    void loadWorldInGame(const std::string& folder);
    void loadWorldInEditor(const std::string& folder);
    void createWorldInGame(WorldProperties& properties);
    void createWorldInEditor(WorldProperties& properties);
    void openMainMenu();
    void closeGame() { worldOpen = false; mainWindow.close(); }
    void closeWorld() { worldOpen = false; }
    void startSimulation(World& world, std::mutex& worldMutex);
    void startNet();
    //
    MainWindow& getMainWindow() { return mainWindow; }
    World& getWorld() { return *_world; }
    GUI& getGUI() { return *_gui; }
    //
    void setPaused(const bool flag) { paused = flag; pauseStart = mainWindow.getTime(); }
    bool isPaused() const { return paused; }
};
