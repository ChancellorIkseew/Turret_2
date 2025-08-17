#pragma once
#include <vector>
#include "engine/coords/tile_coord.hpp"
#include "game/world/world.hpp"

class WorldMap;

struct WorldProperties {
    WorldProperties() = default;
    WorldProperties(const TileCoord mapSize, const uint64_t seed) :
        mapSize(mapSize), seed(seed) { }
    TileCoord mapSize = TileCoord(20, 20);
    uint64_t seed = 0U;
};

namespace gen {
    WorldMap generateMap(const WorldProperties& properties);
    std::unique_ptr<World> generateWorld(const WorldProperties& properties);
}
