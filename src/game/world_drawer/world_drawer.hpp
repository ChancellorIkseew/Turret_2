#pragma once
#include "entities_drawer.hpp"
#include "map_drawer.hpp"
// TODO: #include "weather_drawer.hpp"

class Camera;
class World;

class WorldDrawer {
    MapDrawer mapDrawer;
    EntitiesDrawer entitiesDrawer;
    // TODO: WeatherDrawer weatherDrawer;
public:
    WorldDrawer(const Camera& camera, const World& world) :
        mapDrawer(camera, world), entitiesDrawer(camera, world) {

    }

    void draw() {
        mapDrawer.draw();
        entitiesDrawer.draw();
        // TODO: weatherDrawer.draw();
    }
};
