#pragma once
#include "entities_drawer.hpp"
#include "game/world/world.hpp"
#include "map_drawer.hpp"
// TODO: #include "weather_drawer.hpp"

class Camera;
class Renderer;

class WorldDrawer {
    MapDrawer mapDrawer;
    EntitiesDrawer entitiesDrawer;
    // TODO: WeatherDrawer weatherDrawer;
public:
    WorldDrawer(Atlas& atlas) : mapDrawer(atlas) { }

    void draw(const Camera& camera, const Renderer& renderer, World& world) {
        mapDrawer.draw(camera, renderer, world.getMap());
        entitiesDrawer.draw(camera, renderer, world.getMobs().getSoa(), world.getShells().getSoa());
        // TODO: weatherDrawer.draw();
    }
};
