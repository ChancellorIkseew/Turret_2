#pragma once
#include <memory>
#include "engine_state.hpp"
#include "engine/window/window.hpp"
#include "game/world/world.hpp"

class Engine {
    MainWindow mainWindow;
    EngineState state = EngineState::main_menu;
public:
    Engine(const std::string& windowTitle) : mainWindow(windowTitle) { }
    void run();
    void gameplay();
    void editor();
    void menu();
};
