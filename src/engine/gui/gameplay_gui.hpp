#pragma once
#include "gui.hpp"
//
#include "engine/engine_state.hpp"
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
#include "game/world/world.hpp"

class GameplayGUI : public GUI {
    EngineState& state;
public:
    GameplayGUI(MainWindow& mainWindow, EngineState& state) :
        GUI(mainWindow), state(state) {
        containers.push_back(frontend::initTimer());
        GUI::relocateContainers();
    }
    ~GameplayGUI() final = default;

    void callback() final {
        if (Input::jactive(Escape) && overlaped.empty())
            GUI::addOverlaped(frontend::initMenu(state, *this));
        else
            GUI::callback();
    }
};
