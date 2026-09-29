#pragma once
#include <cstddef>
#include <cstdint>
#include "engine/coords/tile_coord.hpp"

struct WorldConfig {
    TileCoord mapSize = TileCoord(20, 20);
    uint64_t seed = 0;
    uint64_t ticksPerWave = 10800; // 3 minutes.
    uint32_t enemyCountMul = 1;
    int16_t blockCostMul = 1;
    bool toggleWaves = true;
};
