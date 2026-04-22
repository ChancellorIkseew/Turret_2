#pragma once
#include "gui.hpp"
//
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
#include "game/frontend/fr_jei.hpp"

class GameplayGUI : public GUI {
    JEI* jei = nullptr;
public:
    GameplayGUI(Engine& engine) : GUI(engine) {
        jei = frontend::initJEI(engine, JEIContent::only_blocks).release();
        mainCanvas.addToMainLayer(frontend::initTimer(engine));
        mainCanvas.addToMainLayer(frontend::initHint(engine));
        mainCanvas.addToMainLayer(std::unique_ptr<Container>(jei));
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
            jei->drawBlock(renderer, input.getMouseCoord());
        GUI::draw(renderer, atlas);
    }
};
