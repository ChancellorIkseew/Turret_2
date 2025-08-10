#pragma once
#include <vector>
#include "engine/coords/tile_coord.hpp"
#include "game/world/world.hpp"

class WorldMap;

struct WorldProperties {
    WorldProperties(const TileCoord mapSize, const uint64_t seed) :
        mapSize(mapSize), seed(seed) { }
    const TileCoord mapSize;
    const uint64_t seed;
};

namespace gen {
    void generate(WorldMap& map, const WorldProperties& properties);
}
