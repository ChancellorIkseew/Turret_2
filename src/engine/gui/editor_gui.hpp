#pragma once
#include "gui.hpp"
//
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
#include "game/frontend/fr_jei.hpp"
//#include "game/events/events.hpp" //?

class EditorGUI : public GUI {
    JEI* jei = nullptr;
public:
    EditorGUI(Engine& engine) : GUI(engine) {
        jei = frontend::initJEI(engine).release();
        mainCanvas.addToMainLayer(std::unique_ptr<Container>(jei));
    }
    ~EditorGUI() final = default;

    void callback() final {
        if (input.jactive(Escape) && !mainCanvas.hasOverlay())
            GUI::addToOverlay(frontend::initMenu(engine));
        else
            GUI::callback();
    }

    void draw(const Renderer& renderer, const Atlas& atlas) final {
        if (!ownsMouse())
            jei->drawBlock(renderer, input.getMouseCoord());
        GUI::draw(renderer, atlas);
    }
};
