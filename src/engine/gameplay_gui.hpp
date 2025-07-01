#pragma once
#include "gui.hpp"
//
#include "engine/engine_state.hpp"
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
#include "game/events/events.hpp"
#include "game/world/camera.hpp"
#include "game/world/world.hpp"

class GameplayGUI : public GUI {
    Container* menu;
public:
    GameplayGUI(MainWindow& mainWindow, EngineState& state) : GUI(mainWindow) {
        auto c = frontend::initMenu(state);
        menu = c.get();
        menu->setVisible(false);
        containers.push_back(std::move(c));
        containers.push_back(frontend::initSettings());
        containers.push_back(frontend::initTimer());
        containers.push_back(frontend::initEditor());
        GUI::relocateContainers();
    }
    ~GameplayGUI() final = default;

    void callback() final {
        GUI::acceptHotkeys();
        if (Input::jactive(BindName::Escape))
            menu->setVisible(!menu->isVisible());
    }
};
