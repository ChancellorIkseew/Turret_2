#pragma once
#include "gui.hpp"
//
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"

class GameplayGUI : public GUI {
public:
    GameplayGUI(Engine& engine) : GUI(engine) {
        mainCanvas.addToMainLayer(frontend::initTimer(engine));
        mainCanvas.addToMainLayer(frontend::initHint(engine));
        mainCanvas.addToMainLayer(frontend::initJEI(engine));
    }
    ~GameplayGUI() final = default;

    void callback() final {
        if (input.jactive(Escape) && !mainCanvas.hasOverlay())
            GUI::addToOverlay(frontend::initMenu(engine));
        else
            GUI::callback();
        if (input.jactive(Pause))
            engine.getSession().setPaused(!engine.getSession().isPaused(), engine);
    }
};
