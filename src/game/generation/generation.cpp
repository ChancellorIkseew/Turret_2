#include "generation.hpp"
//
#include <algorithm>
#include <vector>
#include "engine/util/local_time.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "game/world/world_map.hpp"
#include "perlin_noise.hpp"
#include <iostream>

constexpr float MAIN_NOISE_SCALE = 40.0f;
constexpr float SUPPORT_NOISE_SCALE = 10.0f;

using Pair = std::pair<float, uint8_t>;

static inline bool fromMaxToMin(const Pair a, const Pair b) {
    return a.first > b.first;
}

static std::vector<Pair> readGen(const ElementRegistry& reg) {
    tin::Data data = tin::read(io::folders::GENERATION / "floor.tin");
    std::vector<Pair> vals;
    for (const auto& [name, val] : data) {
        const float f = data.getFloat(name.c_str()).value_or(1.0f);
        vals.emplace_back(f, reg.floorTypes.at(name));
    }

    std::sort(vals.begin(), vals.end(), fromMaxToMin);

    for (const auto& [f, ui] : vals) {
        std::cout << "float: " << f << " uint: " << ui << '\n';
    }
    return vals;
}

static uint8_t calculateTileType(float height, std::vector<Pair> vals) {
    for (const auto/*not ref*/ [minHeight, floorID] : vals) {
        if (height > minHeight)
            return floorID;
    }
    return 0U;
}

static WorldMap generateMap(const WorldProperties& properties) {
    //if (properties.seed == 0U)
        //properties.seed = t1_time::getUTC();
    const auto mapSize = properties.mapSize;
    PerlinNoise2D mainNoise(properties.seed);
    PerlinNoise2D supportNoise(properties.seed + 100U);
    WorldMap map(mapSize);
    const auto vals = readGen(map.getContent());

    //float min = 1.0f, max = 0.0f;
    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            const float m = mainNoise.createTile(x, y + 1, MAIN_NOISE_SCALE);
            const float s = supportNoise.createTile(x, y + 1, SUPPORT_NOISE_SCALE);
            map.at(x, y).floor = calculateTileType(m * 0.85f + s * 0.25f, vals);
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
