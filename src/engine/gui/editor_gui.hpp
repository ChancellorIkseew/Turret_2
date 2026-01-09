#pragma once
#include "gui.hpp"
//
#include "engine/engine.hpp"
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
#include "game/events/events.hpp"
#include "game/player/camera.hpp"
#include "game/world/world_map.hpp"

class EditorGUI : public GUI {
public:
    EditorGUI(Engine& engine) : GUI(engine) {
        containers.push_back(frontend::initJEI(engine));
        GUI::relocateContainers();
        GUI::translate();
    }
    ~EditorGUI() final = default;

    void callback() final {
        if (input.jactive(Escape) && overlaped.empty())
            GUI::addOverlaped(frontend::initMenu(engine));
        else
            GUI::callback();
    }
};
