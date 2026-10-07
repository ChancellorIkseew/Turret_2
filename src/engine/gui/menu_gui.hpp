#pragma once
#include "gui.hpp"
//
#include "game/frontend/frontend.hpp"

class MenuGUI : public GUI {
public:
    MenuGUI(App& app) : GUI(app) { init(app); }

    void init(App& app) final {
        mainCanvas.closeAll();
        mainCanvas.addToOverlay(frontend::initMainMenu(app));
        mainCanvas.setAllwaysWithOverlay(true);
    }

    void drawDiegeticElements(Renderer& renderer) final {
        /*TODO: add built in logo world*/
    }
};
