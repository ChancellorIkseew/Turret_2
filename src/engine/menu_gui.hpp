#pragma once
#include "gui.hpp"
//
#include "game/frontend/frontend.hpp"
#include "engine/window/window.hpp"

enum class EngineState : uint8_t;

class MenuGUI : public GUI {
public:
    MenuGUI(MainWindow& mainWindow, EngineState& state) : GUI() {
        containers.push_back(frontend::initMainMenu(state));
        relocateContainers(mainWindow.getSize());
    }
    ~MenuGUI() final = default;
};
