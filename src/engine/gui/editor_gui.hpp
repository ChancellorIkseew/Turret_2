#pragma once
#include "gui.hpp"
//
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
//#include "game/events/events.hpp" //?

class EditorGUI : public GUI {
public:
    EditorGUI(Engine& engine) : GUI(engine) {
        mainCanvas.addToMainLayer(frontend::initJEI(engine));
    }
    ~EditorGUI() final = default;

    void callback() final {
        if (input.jactive(Escape) && !mainCanvas.hasOverlay())
            GUI::addToOverlay(frontend::initMenu(engine));
        else
            GUI::callback();
    }
};
