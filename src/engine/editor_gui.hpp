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

class EditorGUI : public GUI {
    TileType tileType = TileType::SNOW;
public:
    EditorGUI(MainWindow& mainWindow, EngineState& state) : GUI() {
        containers.push_back(frontend::initMenu(state));
        containers.push_back(frontend::initJEI(tileType));
        relocateContainers(mainWindow.getSize());
    }
    ~EditorGUI() final = default;

    void editMap(World& world, const Camera& camera) const {
        if (!Input::active(BindName::Build) || !isMouseFree())
            return;
        const TileCoord tile = t1::tile(camera.fromScreenToMap(Input::getMouseCoord()));
        world.placeTile(tile, tileType);
        Events::pushEvent(Event::terrain_changed);
    }
};
