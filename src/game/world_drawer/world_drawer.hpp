#pragma once
#include "entities_drawer.hpp"
#include "game/world/world.hpp"
#include "map_drawer.hpp"
// TODO: #include "weather_drawer.hpp"

class Camera;
class Presets;
class Renderer;

class WorldDrawer {
    MapDrawer mapDrawer;
    EntitiesDrawer entitiesDrawer;
    // TODO: WeatherDrawer weatherDrawer;
public:
    WorldDrawer(const Assets& assets) : mapDrawer(assets) { }

    void draw(const Camera& camera, const Renderer& renderer, World& world,
        const Presets& presets, const uint64_t tickCount) {
        mapDrawer.draw(camera, renderer, world.getMap());
        entitiesDrawer.draw(camera, renderer, world.getMobs().getSoa(), world.getShells().getSoa(), presets, tickCount);
        // TODO: weatherDrawer.draw();
    }
};
