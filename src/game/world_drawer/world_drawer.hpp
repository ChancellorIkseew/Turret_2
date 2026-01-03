#pragma once
#include "entities_drawer.hpp"
#include "game/world/world.hpp"
#include "map_drawer.hpp"
// TODO: #include "weather_drawer.hpp"

class Camera;

class WorldDrawer {
    MapDrawer mapDrawer;
    EntitiesDrawer entitiesDrawer;
    // TODO: WeatherDrawer weatherDrawer;
public:
    WorldDrawer() = default;

    void draw(const Camera& camera, const World& world) {
        mapDrawer.draw(camera, world.getMap());
        entitiesDrawer.draw(camera, world.getMobs().getSoa(), world.getShells().getSoa());
        // TODO: weatherDrawer.draw();
    }
};
