#pragma once
#include "gui.hpp"
//
#include "engine/window/input/input.hpp"
#include "game/frontend/build_tools/gameplay_build_tools.hpp"
#include "game/frontend/frontend.hpp"
#include "game/game_session.hpp"

class GameplayGUI : public GUI {
    std::shared_ptr<BuildTools> buildTools;
public:
    GameplayGUI(App& app) : GUI(app) { init(app); }

    void init(App& app) final {
        mainCanvas.closeAll();
        buildTools = std::make_unique<GameplayBuildTools>();
        mainCanvas.addToMainLayer(frontend::initTimer(app));
        mainCanvas.addToMainLayer(frontend::initHint(app));
        mainCanvas.addToMainLayer(frontend::initInventory(app));
        mainCanvas.addToMainLayer(frontend::initJEI(app, buildTools));
    }

    void callback() final {
        buildTools->update(app);
        if (input.jactive(Pause) && !mainCanvas.hasOverlay())
            app.getSession().setPaused(!app.getSession().isPausedManually(), app);
        if (input.jactive(Escape) && !mainCanvas.hasOverlay())
            return GUI::addToOverlay(frontend::initMenu(app));
        GUI::callback();
    }

    void drawDiegeticElements(Renderer& renderer) final {
        buildTools->drawDraft(app, renderer, app.getMainWindow().getTimeMs());
    }
};
