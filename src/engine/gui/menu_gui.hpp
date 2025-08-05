#pragma once
#include "gui.hpp"
//
#include "engine/engine_state.hpp"
#include "game/frontend/frontend.hpp"

enum class EngineState : uint8_t;

class MenuGUI : public GUI {
public:
    MenuGUI(MainWindow& mainWindow, EngineState& state) : GUI(mainWindow) {
        containers.push_back(frontend::initMainMenu(state, *this));
        GUI::relocateContainers();
    }
    ~MenuGUI() final = default;

    void callback() final {
        GUI::callback();
    }
};
