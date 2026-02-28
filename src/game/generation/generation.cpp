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

struct PreparedFloor { uint8_t type; float height; };
struct PreparedOverlay { uint8_t type; int frequency; int deposite; };

static inline bool fromMaxToMin(const PreparedFloor a, const PreparedFloor b) noexcept {
    return a.height > b.height;
}

static uint8_t calculateTileType(const float height, const std::vector<PreparedFloor>& vals) noexcept {
    for (const auto/*not ref*/ [floorID, minHeight] : vals) {
        if (height >= minHeight)
            return floorID;
    }
    return 0U;
}

static auto prepareFloorPresets(const FloorPresets& floorPresets, const Indexes& indexes) {
    std::vector<PreparedFloor> result;
    for (const auto& [name, height] : floorPresets) {
        if (!indexes.getFloor().contains(name)) {
            logger.warning() << "Content is not registred: \"" << name << "\"";
            continue;
        }
        result.emplace_back(indexes.getFloor().at(name), height);
    }
    std::sort(result.begin(), result.end(), fromMaxToMin);
    return result;
}

static auto prepareOverlayPresets(const OverlayPresets& overlayPresets, const Indexes& indexes) {
    std::vector<PreparedOverlay> result;
    for (const auto& [name, frequency, deposite] : overlayPresets) {
        if (!indexes.getOverlay().contains(name)) {
            logger.warning() << "Content is not registred: \"" << name << "\"";
            continue;
        }
        result.emplace_back(indexes.getOverlay().at(name), frequency, deposite);
    }
    return result;
}

static WorldMap generateMap(const WorldProperties& properties, const Indexes& indexes) {
    const auto mapSize = properties.mapSize;
    PerlinNoise2D mainNoise(properties.seed);
    PerlinNoise2D supportNoise(properties.seed + 100U);
    WorldMap map(mapSize);
    const auto floorPresets = prepareFloorPresets(properties.floorPresets, indexes);
    const auto overlayPresets = prepareOverlayPresets(properties.overlayPresets, indexes);

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
