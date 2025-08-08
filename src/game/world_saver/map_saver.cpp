#include "map_saver.hpp"
//
#include <ZLIB/zlib.h>
#include <fstream>
#include "engine/debug/logger.hpp"
#include "game/world/world.hpp"

constexpr int COMPRESSION_LEVEL = 6;
static debug::Logger logger("map_saver");

void MapSaver::save(const World& world, const std::filesystem::path& path) {
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

    std::ofstream fout(path / "world_map.dat", std::ios::binary);
    fout.write(reinterpret_cast<const char*>(&compressedSize), sizeof(uLongf));
    fout.write(reinterpret_cast<const char*>(compressedData.data()), compressedSize);
    logger.info() << "World map successfully saved.";
}

void MapSaver::load(World& world, const std::filesystem::path& path) {
    std::ifstream fin(path / "world_map.dat", std::ios::binary);
    if (!fin.is_open()) {
        logger.error() << "Failed to open file: " << path / "world_map.dat";
        return;
    }

    uLongf compressedSize = 0;
    fin.read(reinterpret_cast<char*>(&compressedSize), sizeof(compressedSize));

    std::vector<uint8_t> compressedData(compressedSize);
    fin.read(reinterpret_cast<char*>(compressedData.data()), compressedSize);

    std::vector<uint8_t> rawData(200 * 200);
    uLongf destLen = static_cast<uLongf>(rawData.size());
    int result = uncompress(rawData.data(), &destLen, compressedData.data(), compressedSize);
    if (result != Z_OK) {
        logger.error() << "Data uncompression failed.";
        return;
    }

    auto& map = world.getMap();
    for (uint32_t y = 0; y < 200; ++y) {
        for (uint32_t x = 0; x < 200; ++x) {
            map[x][y].floor = rawData[y * 200 + x];
        }
    }
}
