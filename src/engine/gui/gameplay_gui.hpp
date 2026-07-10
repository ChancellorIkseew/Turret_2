#pragma once
#include "gui.hpp"
//
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/window/input/input.hpp"
#include "game/frontend/build_tools/gameplay_build_tools.hpp"
#include "game/frontend/frontend.hpp"

class GameplayGUI : public GUI {
    std::shared_ptr<BuildTools> buildTools;
public:
    GameplayGUI(Engine& engine) : GUI(engine) {
        buildTools = std::make_unique<GameplayBuildTools>();
        mainCanvas.addToMainLayer(frontend::initTimer(engine));
        mainCanvas.addToMainLayer(frontend::initHint(engine));
        mainCanvas.addToMainLayer(frontend::initJEI(engine, buildTools));
    }

    void callback() final {
        buildTools->update(engine);
        if (input.jactive(Pause) && !mainCanvas.hasOverlay())
            engine.getSession().setPaused(!engine.getSession().isPausedManually(), engine);
        if (input.jactive(Escape) && !mainCanvas.hasOverlay())
            return GUI::addToOverlay(frontend::initMenu(engine));
        GUI::callback();
    }

    void drawDiegeticElements(Renderer& renderer) final {
        buildTools->drawDraft(engine, renderer, engine.getMainWindow().getTimeMs());
    }
};
