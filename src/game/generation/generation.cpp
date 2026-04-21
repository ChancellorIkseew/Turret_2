#include "generation.hpp"
//
#include <algorithm>
#include "engine/assets/assets.hpp"
#include "engine/debug/logger.hpp"
#include "game/world/world.hpp"
#include "perlin_noise.hpp"
#include "spot_generator.hpp"
#include "squirell_noise.hpp"

constexpr float MAIN_NOISE_SCALE = 40.0f;
constexpr float SUPPORT_NOISE_SCALE = 10.0f;
constexpr uint32_t SEED_OFFSET = 50U;
static debug::Logger logger("world_generation");

struct PreparedFloor { uint8_t type; float height; };
struct PreparedOverlay { uint8_t type; int frequency; int deposit; };

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

static auto prepareOverlayPresets(const OverlayPresets& overlayPresets, const Presets& presets) {
    std::vector<PreparedOverlay> result;
    for (const auto& [name, frequency, deposit] : overlayPresets) {
        if (!presets.getOres().contains(name)) {
            logger.warning() << "Content is not registred: \"" << name << "\"";
            continue;
        }
        result.emplace_back(presets.getOres().at(name).asUint(), frequency, deposit);
    }
    return result;
}

static WorldMap generateMap(const WorldProperties& properties, const Assets& assets) {
    const auto mapSize = properties.mapSize;
    const auto floorPresets = prepareFloorPresets(properties.floorPresets, assets.getIndexes());
    const auto overlayPresets = prepareOverlayPresets(properties.overlayPresets, assets.getPresets());
    //
    const PerlinNoise2D mainNoise(properties.seed);
    const PerlinNoise2D supportNoise(properties.seed + 100U);
    const SquirellNoise2D squirellNoise(properties.seed);
    const SpotGenerator2D spotGenerator(properties.seed);
    //
    std::vector<uint8_t>   floor(mapSize.x * mapSize.y);
    std::vector<OrePresetID> ore(mapSize.x * mapSize.y);
    //
    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            const float m = mainNoise.createTile(x, y + 1, MAIN_NOISE_SCALE);
            const float s = supportNoise.createTile(x, y + 1, SUPPORT_NOISE_SCALE);
            floor[x + y * mapSize.x] = calculateTileType(m * 0.85f + s * 0.25f, floorPresets);

            for (const auto& [id, frequency, deposit] : overlayPresets) {
                const uint32_t seedOffset = id * SEED_OFFSET;
                if (squirellNoise.createTile(x, y, frequency, seedOffset))
                    spotGenerator.generateSpot(ore, TileCoord(x, y), OrePresetID(id), deposit, mapSize);
            }
        }
    }
    return WorldMap(mapSize, floor, ore);
}

std::unique_ptr<World> gen::generateWorld(const WorldProperties& properties, const Assets& assets) {
    auto map = generateMap(properties, assets);
    return std::make_unique<World>(map);
}
