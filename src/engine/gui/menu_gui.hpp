#pragma once
#include "gui.hpp"
//
#include "engine/engine.hpp"
#include "game/frontend/frontend.hpp"
#include "game/generation/generation.hpp"

enum class EngineState : uint8_t;

class MenuGUI : public GUI {
public:
    MenuGUI(Engine& engine) : GUI(engine) {
        overlaped.push_back(frontend::initMainMenu(engine));
        GUI::relocateContainers();
        GUI::translate();
    }
    ~MenuGUI() final = default;

    void callback() final {
        GUI::acceptHotkeys();
        if (!showGUI)
            return;
        overlaped.back()->callback(input);
        if (overlaped.size() > 1 && (!overlaped.back()->isOpen() || input.jactive(Escape)))
            overlaped.pop_back();
    }
};
