#pragma once
#include "gui.hpp"
//
#include "engine/engine_state.hpp"
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
#include "game/events/events.hpp"
#include "game/world/camera.hpp"
#include "game/world/world.hpp"
#include "gui_util/tile_data.hpp"

class EditorGUI : public GUI {
    Container* menu;
    const Camera& camera;
    WorldMap& map;
    TileData tileData;
public:
    EditorGUI(MainWindow& mainWindow, EngineState& state, WorldMap& map, Camera& camera) :
        GUI(mainWindow), map(map), camera(camera) {
        containers.push_back(frontend::initJEI(tileData, map.getContent()));
        menu = containers.emplace_back(frontend::initMenu(state)).get();
        menu->setVisible(false);
        GUI::relocateContainers();
    }
    ~EditorGUI() final = default;

    void callback() final {
        GUI::acceptHotkeys();
        if (Input::jactive(Escape))
            menu->setVisible(!menu->isVisible());
        editMap();
    }
private:
    void editMap() const {
        if (!Input::active(Build) || !GUI::isMouseFree())
            return;
        const TileCoord tile = t1::tile(camera.fromScreenToMap(Input::getMouseCoord()));
        switch (tileData.component) {
        case TileComponent::floor:   map.placeFloor(tile, tileData.id);   break;
        case TileComponent::overlay: map.placeOverlay(tile, tileData.id); break;
        case TileComponent::block: break;
        }
        Events::pushEvent(Event::terrain_changed);
    }
};
