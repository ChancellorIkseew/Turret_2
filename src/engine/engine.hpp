#pragma once
#include "engine_command.hpp"
#include "engine/assets/assets.hpp"
#include "engine/scripting/scripting.hpp"
#include "engine/window/window.hpp"
#include "game/generation/generation.hpp"

class Camera;
class GUI;
class PlayerController;
class World;

class Engine {
    MainWindow mainWindow;
    Assets assets;
    ScriptsHandler scriptsHandler;
    WorldProperties worldProperties;
    std::string worldFolder;
    PlayerController* _playerController = nullptr;
    Camera* _camera = nullptr;
    World*  _world  = nullptr;
    GUI*    _gui    = nullptr;
    EngineCommand command = EngineCommand::main_menu;
    bool worldOpen = false, paused = false;
public:
    Engine(const std::string& windowTitle) : mainWindow(windowTitle) { }
    void run();
    void createScene(const std::string& folder, WorldProperties& properties);
    void loadWorldInGame(const std::string& folder);
    void loadWorldInEditor(const std::string& folder);
    void createWorldInGame(WorldProperties& properties);
    void createWorldInEditor();
    void openMainMenu();
    void closeWorld() { worldOpen = false; }
    void closeGame() { closeWorld(); mainWindow.close(); }
    void startNet();
    //
    const PlayerController& getPlayerController() const { return *_playerController; }
    const MainWindow& getMainWindow() const { return mainWindow; }
    const Camera& getCamera() const { return *_camera; }
    const World& getWorld() const { return *_world; }
    const GUI& getGUI() const { return *_gui; }
    PlayerController& getPlayerController() { return *_playerController; }
    MainWindow& getMainWindow() { return mainWindow; }
    Camera& getCamera() { return *_camera; }
    World& getWorld() { return *_world; }
    GUI& getGUI() { return *_gui; }
    Assets& getAssets() { return assets; };
    //
    bool isWorldOpen() const { return worldOpen; }
    bool isPaused() const { return paused; }
    void setPaused(const bool flag);
};
