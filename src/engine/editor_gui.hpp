#pragma once
#include "gui.hpp"
//
#include "engine/engine_state.hpp"
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
#include "game/events/events.hpp"
#include "game/world/camera.hpp"
#include "game/world/world.hpp"
#include "tile_data.hpp"

class EditorGUI : public GUI {
    Container* menu;
    const Camera& camera;
    World& world;
    TileData tileData;
public:
    EditorGUI(MainWindow& mainWindow, EngineState& state, World& world, Camera& camera) :
        GUI(mainWindow), world(world), camera(camera) {
        containers.push_back(frontend::initJEI(tileData, world.getContent()));
        menu = containers.emplace_back(frontend::initMenu(state)).get();
        menu->setVisible(false);
        GUI::relocateContainers();
    }
    ~EditorGUI() final = default;

    void callback() final {
        GUI::acceptHotkeys();
        if (Input::jactive(BindName::Escape))
            menu->setVisible(!menu->isVisible());
        editMap();
    }
private:
    void editMap() const {
        if (!Input::active(BindName::Build) || !GUI::isMouseFree())
            return;
        const TileCoord tile = t1::tile(camera.fromScreenToMap(Input::getMouseCoord()));
        switch (tileData.component) {
        case TileComponent::floor:   world.placeFloor(tile, tileData.id);   break;
        case TileComponent::overlay: world.placeOverlay(tile, tileData.id); break;
        case TileComponent::block: break;
        }
        Events::pushEvent(Event::terrain_changed);
    }
};
