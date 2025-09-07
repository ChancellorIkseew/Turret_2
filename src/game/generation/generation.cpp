#include "generation.hpp"
//
#include "engine/util/local_time.hpp"
#include "game/world/world_map.hpp"
#include "perlin_noise.hpp"
//#include <iostream>

constexpr float MAIN_NOISE_SCALE = 40.0f;
constexpr float SUPPORT_NOISE_SCALE = 10.0f;

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

static WorldMap generateMap(const WorldProperties& properties) {
    //if (properties.seed == 0U)
        //properties.seed = t1_time::getUTC();
    const auto mapSize = properties.mapSize;
    PerlinNoise2D mainNoise(properties.seed);
    PerlinNoise2D supportNoise(properties.seed + 100U);
    WorldMap map(mapSize);

    //float min = 1.0f, max = 0.0f;
    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            const float m = mainNoise.createTile(x, y + 1, MAIN_NOISE_SCALE);
            const float s = supportNoise.createTile(x, y + 1, SUPPORT_NOISE_SCALE);
            map[x][y].floor = calculateTileType(m * 0.85f + s * 0.25f);
            //min = std::min(mainNoise.createTile(x, y + 1, 40.0f), min);
            //max = std::max(mainNoise.createTile(x, y + 1, 40.0f), max);
        }
    }

    //std::cout << "min: " << min << " max: "<< max << '\n';
    return map;
}

std::unique_ptr<World> gen::generateWorld(const WorldProperties& properties) {
    auto map = generateMap(properties);
    return std::make_unique<World>(map);
}
