#pragma once
#include "gui.hpp"
//
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/window/input/input.hpp"
#include "game/frontend/build_tools/build_tools.hpp"
#include "game/frontend/frontend.hpp"

class GameplayGUI : public GUI {
    std::shared_ptr<BuildTools> buildTools;
public:
    GameplayGUI(Engine& engine) : GUI(engine) {
        buildTools = std::make_unique<BuildTools>(JEIContent::only_blocks);
        mainCanvas.addToMainLayer(frontend::initTimer(engine));
        mainCanvas.addToMainLayer(frontend::initHint(engine));
        mainCanvas.addToMainLayer(frontend::initJEI(engine, buildTools));
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

    void draw(const Renderer& renderer, const Atlas& atlas) final {
        if (!ownsMouse())
            buildTools->drawBlueprint(engine, renderer);
        GUI::draw(renderer, atlas);
    }
};
