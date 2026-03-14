#pragma once
#include "gui.hpp"
//
#include "game/frontend/frontend.hpp"

enum class EngineState : uint8_t;

class MenuGUI : public GUI {
public:
    MenuGUI(Engine& engine) : GUI(engine) {
        mainCanvas.addToOverlay(frontend::initMainMenu(engine));
        mainCanvas.setAllwaysWithOverlay(true);
    }
    ~MenuGUI() final = default;
};
