#include "map_saver.hpp"
//
#include <ZLIB/zlib.h>
#include <fstream>
#include "engine/debug/logger.hpp"
#include "game/world/world_map.hpp"

constexpr int COMPRESSION_LEVEL = 6;
static debug::Logger logger("map_saver");

void MapSaver::save(const WorldMap& map, const std::filesystem::path& path) {
    const auto mapSize = map.getSize();
    uLong tileCount = static_cast<uLong>(mapSize.x * mapSize.y);
    std::vector<uint8_t> rawData;
    rawData.reserve(tileCount);
    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            rawData.push_back(map[x][y].floor);
        }
    }

    uLongf packedDataSize = compressBound(tileCount);
    std::vector<uint8_t> packedData(packedDataSize);

    int result = compress2(packedData.data(), &packedDataSize, rawData.data(), static_cast<uLong>(tileCount), COMPRESSION_LEVEL);
    if (result != Z_OK) {
        logger.error() << "Data compression failed. Error code: " << result;
        return;
    }

    std::ofstream fout(path / "world_map.dat", std::ios::binary);
    fout.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
    fout.write(reinterpret_cast<const char*>(&packedDataSize), sizeof(packedDataSize));
    fout.write(reinterpret_cast<const char*>(packedData.data()), packedDataSize);
    logger.info() << "World map successfully saved.";
}

WorldMap MapSaver::load(const std::filesystem::path& path) {
    std::ifstream fin(path / "world_map.dat", std::ios::binary);
    if (!fin.is_open()) {
        logger.error() << "Failed to open file: " << path / "world_map.dat";
        return WorldMap(TileCoord(0, 0));
    }

    TileCoord mapSize;
    uLongf packedDataSize = 0;
    fin.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
    fin.read(reinterpret_cast<char*>(&packedDataSize), sizeof(packedDataSize));

    std::vector<uint8_t> packedData(packedDataSize);
    std::vector<uint8_t> rawData(mapSize.x * mapSize.y);
    fin.read(reinterpret_cast<char*>(packedData.data()), packedDataSize);
    uLongf destLen = static_cast<uLongf>(rawData.size());

    int result = uncompress(rawData.data(), &destLen, packedData.data(), packedDataSize);
    if (result != Z_OK) {
        logger.error() << "Data uncompression failed. Error code: " << result;
        return WorldMap(TileCoord(0, 0));
    }

    WorldMap map(mapSize);
    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            map[x][y].floor = rawData[x * mapSize.y + y];
        }
    }
    logger.info() << "World map successfully load.";
    return map;
}
