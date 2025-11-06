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
    WorldDrawer(const Camera& camera, const World& world) :
        mapDrawer(camera, world.getMap()), entitiesDrawer(camera, world) {

    }

    void draw(const float tickOfset) {
        mapDrawer.draw();
        entitiesDrawer.draw(tickOfset);
        // TODO: weatherDrawer.draw();
    }
};
