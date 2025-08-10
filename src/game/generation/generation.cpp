#include "generation.hpp"
//
#include "engine/util/local_time.hpp"
#include "game/world/world_map.hpp"
#include "perlin_noise.hpp"
#include <iostream>

enum TileType : uint8_t {
    NONE = 0,
    SNOW = 1,
    ICE = 2,
    WATER = 3,
    SOIL = 4,
    ROCK = 5,
    MAGMA = 6
};

static uint8_t calculateTileType(float height) {
    if (height > 0.65f)  return TileType::MAGMA;
    if (height > 0.625f) return TileType::ROCK;
    if (height > 0.6f)   return TileType::SOIL;
    if (height > 0.585f) return TileType::WATER;
    if (height > 0.57f)  return TileType::ICE;
    return TileType::SNOW;
}

void gen::generate(WorldMap& map, const WorldProperties& properties) {
    //if (properties.seed == 0U)
        //properties.seed = t1_time::getUTC();
    const auto mapSize = properties.mapSize;
    PerlinNoise2D noise(properties.seed);

    float min = 1.0f, max = 0.0f;
    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            map[x][y].floor = calculateTileType(noise.createTile(x, y + 1, 40.0f));
            min = std::min(noise.createTile(x, y + 1, 40.0f), min);
            max = std::max(noise.createTile(x, y + 1, 40.0f), max);
        }
    }

    std::cout << "min: " << min << " max: "<< max << '\n';
}
