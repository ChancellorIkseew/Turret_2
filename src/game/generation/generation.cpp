#include "generation.hpp"
//
#include <algorithm>
#include "engine/assets/indexes.hpp"
#include "engine/debug/logger.hpp"
#include "game/world/world.hpp"
#include "perlin_noise.hpp"
#include "spot_generator.hpp"
#include "squirell_noise.hpp"

constexpr float MAIN_NOISE_SCALE = 40.0f;
constexpr float SUPPORT_NOISE_SCALE = 10.0f;
static debug::Logger logger("world_generation");

using Pair = std::pair<float, uint8_t>;

struct OvPr {
    uint8_t type = 0;
    int frequency = 0, deposite = 0;
};

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

static std::vector<Pair> processFloorPresets(const FloorPresets& floorPresets, const Indexes& indexes) {
    std::vector<Pair> vals;
    for (const auto& [name, height] : floorPresets) {
        if (!indexes.getFloor().contains(name)) {
            logger.warning() << "Content is not registred: \"" << name << "\"";
            continue;
        }
        vals.emplace_back(height, indexes.getFloor().at(name));
    }
    std::sort(vals.begin(), vals.end(), fromMaxToMin);
    return vals;
}

static std::vector<OvPr> processOverlayPresets(const OverlayPresets& overlayPresets, const Indexes& indexes) {
    std::vector<OvPr> vals;
    for (const auto& [name, frequency, deposite] : overlayPresets) {
        if (!indexes.getOverlay().contains(name)) {
            logger.warning() << "Content is not registred: \"" << name << "\"";
            continue;
        }
        vals.emplace_back(indexes.getOverlay().at(name), frequency, deposite);
    }
    return vals;
}

static WorldMap generateMap(const WorldProperties& properties, const Indexes& indexes) {
    const auto mapSize = properties.mapSize;
    PerlinNoise2D mainNoise(properties.seed);
    PerlinNoise2D supportNoise(properties.seed + 100U);
    WorldMap map(mapSize);
    const auto floorPresets = processFloorPresets(properties.floorPresets, indexes);
    const auto overlayPresets = processOverlayPresets(properties.overlayPresets, indexes);

    SpotGenerator2D spotGenerator(properties.seed);

    std::unordered_map<uint8_t, SquirellNoise2D> squirellNoises;
    uint64_t seedOfset = 0U;
    for (const auto& [id, frequency, deposite] : overlayPresets) {
        squirellNoises.emplace(id, SquirellNoise2D(properties.seed + seedOfset));
        seedOfset += 50U;
    }

    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            const float m = mainNoise.createTile(x, y + 1, MAIN_NOISE_SCALE);
            const float s = supportNoise.createTile(x, y + 1, SUPPORT_NOISE_SCALE);
            map.at(x, y).floor = calculateTileType(m * 0.85f + s * 0.25f, floorPresets);
 
            for (const auto& [id, frequency, deposite] : overlayPresets) {
                if (squirellNoises.at(id).createTile(x, y, frequency))
                    spotGenerator.generateSpot(map, TileCoord(x, y), id, deposite);
            }
        }
    }
    return map;
}

std::unique_ptr<World> gen::generateWorld(const WorldProperties& properties, const Indexes& indexes) {
    auto map = generateMap(properties, indexes);
    return std::make_unique<World>(map);
}
