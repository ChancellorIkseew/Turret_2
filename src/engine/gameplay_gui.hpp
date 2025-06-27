#pragma once
#include "gui.hpp"
//
#include "engine/window/input/input.hpp"
#include "engine/window/window.hpp"
#include "game/frontend/frontend.hpp"
#include "game/events/events.hpp"
#include "game/world/camera.hpp"
#include "game/world/world.hpp"

enum class EngineState : uint8_t;

class GameplayGUI : public GUI {
public:
    GameplayGUI(MainWindow& mainWindow, EngineState& state) : GUI() {
        containers.push_back(frontend::initMenu(state));
        containers.push_back(frontend::initSettings());
        containers.push_back(frontend::initTimer());
        containers.push_back(frontend::initEditor());
        relocateContainers(mainWindow.getSize());
    }
    ~GameplayGUI() final = default;
};
