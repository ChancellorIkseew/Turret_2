#pragma once
#include <vector>
#include "engine/coords/tile_coord.hpp"
#include "game/world/world.hpp"

namespace gen {
    void generate(std::vector<std::vector<MapTile>>& map, const TileCoord mapSize);
}
