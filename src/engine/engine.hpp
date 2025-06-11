#pragma once
#include "window/window.hpp"
#include "world/world.hpp"

class Engine {
    MainWindow mainWindow;
    std::unique_ptr<World> world;
public:
    Engine(const std::string& windowTitle) : mainWindow(windowTitle) { }
    void run();
};
