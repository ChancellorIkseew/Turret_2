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
        if (input.jactive(Escape) && !mainCanvas.hasOverlay())
            GUI::addToOverlay(frontend::initMenu(engine));
        else
            GUI::callback();
        if (input.jactive(Pause))
            engine.getSession().setPaused(!engine.getSession().isPaused(), engine);
    }

    void drawDiegeticElements(Renderer& renderer) {
        if (!ownsMouse())
            buildTools->drawDraft(engine, renderer);
    }
};
