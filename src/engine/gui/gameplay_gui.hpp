#pragma once
#include "gui.hpp"
//
#include "engine/engine.hpp"
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
#include "game/world/world.hpp"

class GameplayGUI : public GUI {
public:
    GameplayGUI(Engine& engine) : GUI(engine) {
        containers.push_back(frontend::initTimer());
        GUI::relocateContainers();
        GUI::translate();
    }
    ~GameplayGUI() final = default;

    void callback() final {
        if (Input::jactive(Escape) && overlaped.empty())
            GUI::addOverlaped(frontend::initMenu(engine));
        else
            GUI::callback();
    }
};
