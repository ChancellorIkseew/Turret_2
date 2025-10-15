#include "generation.hpp"
//
#include <algorithm>
#include <vector>
#include "engine/debug/logger.hpp"
#include "game/world/world.hpp"
#include "hash_noise.hpp"
#include "perlin_noise.hpp"
#include "spot_generator.hpp"

constexpr float MAIN_NOISE_SCALE = 40.0f;
constexpr float SUPPORT_NOISE_SCALE = 10.0f;

using Pair = std::pair<float, uint8_t>;

static inline bool fromMaxToMin(const Pair a, const Pair b) {
    return a.first > b.first;
}

static uint8_t calculateTileType(const float height, const std::vector<Pair>& vals) {
    for (const auto/*not ref*/ [minHeight, floorID] : vals) {
        if (height >= minHeight)
            return floorID;
    }
    return 0U;
}

static std::vector<Pair> processFloorPresets(const FloorPresets& floorPresets, const WorldMap& map) {
    std::vector<Pair> vals;
    for (const auto& [name, height] : floorPresets) {
        vals.emplace_back(height, map.getContentIndexes().floorTypes.at(name));
    }
    std::sort(vals.begin(), vals.end(), fromMaxToMin);
    return vals;
}

static WorldMap generateMap(const WorldProperties& properties) {
    const auto mapSize = properties.mapSize;
    PerlinNoise2D mainNoise(properties.seed);
    PerlinNoise2D supportNoise(properties.seed + 100U);
    WorldMap map(mapSize);
    const auto vals = processFloorPresets(properties.floorPresets, map);

    SpotGenerator2D spotGenerator(properties.seed);

    std::unordered_map<std::string, HashNoise2D> hashNoises;
    uint64_t seedOfset = 0U;
    for (const auto& [id, frequency, deposite] : properties.overlayPresets) {
        hashNoises.emplace(id, HashNoise2D(properties.seed + seedOfset));
        seedOfset += 50U;
    }

    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            const float m = mainNoise.createTile(x, y + 1, MAIN_NOISE_SCALE);
            const float s = supportNoise.createTile(x, y + 1, SUPPORT_NOISE_SCALE);
            map.at(x, y).floor = calculateTileType(m * 0.85f + s * 0.25f, vals);
 
            for (const auto& [id, frequency, deposite] : properties.overlayPresets) {
                if (hashNoises.at(id).createTile(x, y, frequency))
                    spotGenerator.generateSpot(map, TileCoord(x, y), map.getContentIndexes().overlayTypes.at(id), deposite);
            }
        }
    }
    return map;
}

std::unique_ptr<World> gen::generateWorld(const WorldProperties& properties) {
    auto map = generateMap(properties);
    return std::make_unique<World>(map);
}
