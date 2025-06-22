#pragma once
#include <memory>
#include "engine/window/window.hpp"
#include "game/world/world.hpp"

class Engine {
    MainWindow mainWindow;
    std::unique_ptr<World> world;
public:
    Engine(const std::string& windowTitle) : mainWindow(windowTitle) { }
    void run();
};
