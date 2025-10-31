#pragma once
#include "gui.hpp"
//
#include "engine/engine.hpp"
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
#include "game/events/events.hpp"
#include "game/player/camera.hpp"
#include "game/world/world_map.hpp"
#include "gui_util/tile_data.hpp"

class EditorGUI : public GUI {
    const Camera& camera;
    WorldMap& map;
    TileData tileData;
public:
    EditorGUI(Engine& engine, WorldMap& map, const Camera& camera) :
        GUI(engine), map(map), camera(camera) {
        containers.push_back(frontend::initJEI(tileData));
        GUI::relocateContainers();
        GUI::translate();
    }
    ~EditorGUI() final = default;

    void callback() final {
        if (Input::jactive(Escape) && overlaped.empty())
            GUI::addOverlaped(frontend::initMenu(engine));
        else
            GUI::callback();
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
    }
};
