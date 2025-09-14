#include "generation.hpp"
//
#include <algorithm>
#include <vector>
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/util/local_time.hpp"
#include "game/world/world_map.hpp"
#include "hash_noise.hpp"
#include "perlin_noise.hpp"
#include "spot_generator.hpp"

constexpr float MAIN_NOISE_SCALE = 40.0f;
constexpr float SUPPORT_NOISE_SCALE = 10.0f;
static debug::Logger logger("world_generation");

using Pair = std::pair<float, uint8_t>;
using Pair2 = std::pair<uint8_t, TileCoord>;

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

static std::vector<Pair2> readGen2(const ElementRegistry& reg) {
    tin::Data data = tin::read(io::folders::GENERATION / "overlay.tin");
    std::vector<Pair2> vals;
    for (const auto& [name, val] : data) {
        const auto tc = data.getTileCoord(name.c_str()).value_or(TileCoord());
        vals.emplace_back(reg.overlayTypes.at(name), tc);
    }

    for (const auto& [ui, tc] : vals) {
        logger.debug() << "uint: " << ui << " f: " << tc.x << " d:" << tc.y;
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
    const auto vals2 = readGen2(map.getContent());

    SpotGenerator2D spotGenerator(properties.seed);

    std::unordered_map<uint8_t, HashNoise2D> hashNoises;
    uint64_t x = 0U;
    for (const auto& [ui, tc] : vals2) {
        hashNoises.emplace(ui, HashNoise2D(properties.seed + x));
        x += 50U;
    }

    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            const float m = mainNoise.createTile(x, y + 1, MAIN_NOISE_SCALE);
            const float s = supportNoise.createTile(x, y + 1, SUPPORT_NOISE_SCALE);
            map.at(x, y).floor = calculateTileType(m * 0.85f + s * 0.25f, vals);
 
            for (const auto& [ui, tc] : vals2) {
                if (hashNoises.at(ui).createTile(x, y, tc.x))
                    spotGenerator.generateSpot(map, TileCoord(x, y), ui, tc.y);
            }
        }
    }
    return map;
}

std::unique_ptr<World> gen::generateWorld(const WorldProperties& properties) {
    auto map = generateMap(properties);
    return std::make_unique<World>(map);
}
