#pragma once
#include "engine_state.hpp"
#include "engine/scripting/scripting.hpp"
#include "engine/window/window.hpp"

class Engine {
    MainWindow mainWindow;
    ScriptsHandler scriptsHandler;
    EngineState state = EngineState::main_menu;
public:
    Engine(const std::string& windowTitle) : mainWindow(windowTitle) { }
    void run();
    void createScene(const EngineState requiredState);
};
