#include "map_saver.hpp"
//
#include <ZLIB/zlib.h>
#include <fstream>
#include "engine/debug/logger.hpp"
#include "game/world/world.hpp"

constexpr int COMPRESSION_LEVEL = 6;
static debug::Logger logger("map_saver");

void MapSaver::save(const World& world) {
    const auto& map = world.getMap();
    uLong tileCount = static_cast<uLong>(map.getSize().x * map.getSize().y);
    std::vector<uint8_t> rawData;
    rawData.reserve(tileCount);
    for (int y = 0; y < map.getSize().y; ++y) {
        for (int x = 0; x < map.getSize().x; ++x) {
            rawData.emplace_back(map[y][x].floor);
        }
    }

    uLongf compressedSize = compressBound(tileCount);
    std::vector<uint8_t> compressedData(compressedSize);


    int result = compress2(compressedData.data(), &compressedSize, rawData.data(), static_cast<uLong>(tileCount), COMPRESSION_LEVEL);
    if (result != Z_OK) {
        logger.error() << "Data compression failed.";
        return;
    }

    for (int i = 0; i < compressedData.size(); ++i) {
        

    }



    std::ofstream fout("saves/save_1/world.dat", std::ios::binary);
    fout.write(reinterpret_cast<const char*>(&compressedSize), sizeof(uLongf));
    fout.write(reinterpret_cast<const char*>(compressedData.data()), compressedSize);
}
