#pragma once
#include "gui.hpp"
//
#include "engine/engine_state.hpp"
#include "game/frontend/frontend.hpp"

enum class EngineState : uint8_t;

class MenuGUI : public GUI {
public:
    MenuGUI(MainWindow& mainWindow, EngineState& state) : GUI(mainWindow) {
        overlaped.push_back(frontend::initMainMenu(state, *this));
        GUI::relocateContainers();
    }
    ~MenuGUI() final = default;

    void callback() final {
        GUI::acceptHotkeys();
        if (!showGUI)
            return;
        overlaped.back()->callback();
        if (overlaped.size() > 1 && (!overlaped.back()->isOpen() || Input::jactive(Escape)))
            overlaped.pop_back();
    }
};
