#pragma once
#include "gui.hpp"
//
#include "game/frontend/frontend.hpp"

class MenuGUI : public GUI {
public:
    MenuGUI(Engine& engine) : GUI(engine) { init(engine); }

    void init(Engine& engine) final {
        mainCanvas.closeAll();
        mainCanvas.addToOverlay(frontend::initMainMenu(engine));
        mainCanvas.setAllwaysWithOverlay(true);
    }

    void drawDiegeticElements(Renderer& renderer) final {
        /*TODO: add built in logo world*/
    }
};
