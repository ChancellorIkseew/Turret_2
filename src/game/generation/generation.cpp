#include "generation.hpp"
//
#include <algorithm>
#include <vector>
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "game/world/world.hpp"
#include "hash_noise.hpp"
#include "perlin_noise.hpp"
#include "spot_generator.hpp"

constexpr float MAIN_NOISE_SCALE = 40.0f;
constexpr float SUPPORT_NOISE_SCALE = 10.0f;
static debug::Logger logger("world_generation");

using Pair = std::pair<float, uint8_t>;

static inline bool fromMaxToMin(const Pair a, const Pair b) {
    return a.first > b.first;
}

static std::vector<Pair> readGen(const ElementRegistry& reg) {
    tin::Data data = tin::read(io::folders::GENERATION / "floor.tin");
    std::vector<Pair> vals;
    for (const auto& [name, val] : data) {
        vals.emplace_back(validator::toFloat(val).value_or(1.0f), reg.floorTypes.at(name));
    }

    std::sort(vals.begin(), vals.end(), fromMaxToMin);

    for (const auto& [f, ui] : vals) {
        logger.debug() << "float: " << f << " uint: " << ui;
    }
    return vals;
}

static uint8_t calculateTileType(const float height, const std::vector<Pair>& vals) {
    for (const auto/*not ref*/ [minHeight, floorID] : vals) {
        if (height >= minHeight)
            return floorID;
    }
    return 0U;
}

static WorldMap generateMap(const WorldProperties& properties) {
    const auto mapSize = properties.mapSize;
    PerlinNoise2D mainNoise(properties.seed);
    PerlinNoise2D supportNoise(properties.seed + 100U);
    WorldMap map(mapSize);
    const auto vals = readGen(map.getContent());

    SpotGenerator2D spotGenerator(properties.seed);

    std::unordered_map<uint8_t, HashNoise2D> hashNoises;
    uint64_t x = 0U;
    for (const auto& [id, frequency, deposite] : properties.overlayPresets) {
        hashNoises.emplace(id, HashNoise2D(properties.seed + x));
        x += 50U;
    }

    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            const float m = mainNoise.createTile(x, y + 1, MAIN_NOISE_SCALE);
            const float s = supportNoise.createTile(x, y + 1, SUPPORT_NOISE_SCALE);
            map.at(x, y).floor = calculateTileType(m * 0.85f + s * 0.25f, vals);
 
            for (const auto& [id, frequency, deposite] : properties.overlayPresets) {
                if (hashNoises.at(id).createTile(x, y, frequency))
                    spotGenerator.generateSpot(map, TileCoord(x, y), id, deposite);
            }
        }
    }
    return map;
}

std::unique_ptr<World> gen::generateWorld(const WorldProperties& properties) {
    auto map = generateMap(properties);
    return std::make_unique<World>(map);
}
