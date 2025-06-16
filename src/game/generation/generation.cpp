#include "generation.hpp"
//
#define FNL_IMPL
#include <FastNoiseLite.h>
#define STB_PERLIN_IMPLEMENTATION
#include <STB/stb_perlin.h>
#include "engine/util/local_time.hpp"

static TileType calculateTileType(float height) {
    //if (height < -0.45f) return TileType::WATER_DEEP;
    if (height < -0.5f)  return TileType::WATER_SHALLOW;
    if (height < -0.1f)  return TileType::GRASS;
    //if (height < 0.5f)   return TileType::GRASS;
    return TileType::SAND;
    //if (height < 0.4f)   return TileType::FOREST;
    //if (height < 0.7f)   return TileType::MOUNTAIN_BASE;
    //return TileType::MOUNTAIN_PEAK;
}

void gen::generate(std::vector<std::vector<MapTile>>& map, const TileCoord mapSize) {
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetSeed(static_cast<int>(t1_time::getUTC()));
    int index = 0;

    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.x; ++y) {
            map[x][y].tileType = calculateTileType(noise.GetNoise(static_cast<float>(x), static_cast<float>(y+1)));
            //map[x][y].tileType = (TileType)stb_perlin_noise3_wrap_nonpow2((float)x, (float)y, 1, 2, 2, 1, 110);

            //map[x][y].tileType = (TileType)2;
        }
    }
}