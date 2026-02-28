#pragma once
#include "entities_drawer.hpp"
#include "game/blocks/blocks_system.hpp"
#include "game/world/world.hpp"
#include "map_drawer.hpp"
#include "particles_drawer.hpp"
// TODO: #include "weather_drawer.hpp"

class Atlas;
class Camera;
class Presets;
class Renderer;

class WorldDrawer {
    MapDrawer mapDrawer;
    EntitiesDrawer entitiesDrawer;
    ParticlesDrawer particlesDrawer;
    // TODO: WeatherDrawer weatherDrawer;
public:
    WorldDrawer(const Assets& assets) : mapDrawer(assets), particlesDrawer(assets) { }

    void draw(const Camera& camera, Renderer& renderer, World& world,
        const Presets& presets, const Assets& assets, const uint64_t tickCount) {
        mapDrawer.draw(camera, renderer, world.getMap());
        blocks::drawBlocks(world.getBlocks(), assets, camera, renderer);
        entitiesDrawer.draw(camera, renderer, world.getBlocks(), world.getMobs().getSoa(), world.getShells().getSoa(), presets, tickCount);
        particlesDrawer.draw(camera, renderer, world.getParticles().getSoa());
        // TODO: weatherDrawer.draw();
    }
};
