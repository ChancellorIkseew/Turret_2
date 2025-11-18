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
    std::atomic_bool worldOpen = false, paused = false;
public:
    Engine(const std::string& windowTitle) : mainWindow(windowTitle) { }
    void run();
    void createScene(const std::string& folder, WorldProperties& properties);
    void loadWorldInGame(const std::string& folder);
    void loadWorldInEditor(const std::string& folder);
    void createWorldInGame(WorldProperties& properties);
    void createWorldInEditor();
    void openMainMenu();
    void closeWorld() { worldOpen.store(false, std::memory_order::seq_cst); }
    void closeGame() { closeWorld(); mainWindow.close(); }
    void startSimulation(World& world, std::mutex& worldMutex);
    void startNet();
    //
    MainWindow& getMainWindow() { return mainWindow; }
    World& getWorld() { return *_world; }
    GUI& getGUI() { return *_gui; }
    //
    bool isWorldOpen() const { return worldOpen.load(std::memory_order_relaxed); }
    bool isPaused() const { return paused.load(std::memory_order_relaxed); }
    void setPaused(const bool flag) {
        paused.store(flag, std::memory_order::seq_cst);
        pauseStart = mainWindow.getTime();
    }
};
