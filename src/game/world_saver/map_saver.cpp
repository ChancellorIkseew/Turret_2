#include "map_saver.hpp"
//
#include <ZLIB/zlib.h>
#include <filesystem>
#include <fstream>
#include "engine/debug/logger.hpp"
#include "game/world/world_map.hpp"
#include "serializer_error.hpp"

constexpr int COMPRESSION_LEVEL = 6;
static debug::Logger logger("serializer");

void serializer::saveMap(const WorldMap& map, const std::filesystem::path& path) {
    const auto mapSize = map.getSize();
    const int tileCount = mapSize.x * mapSize.y;
    std::vector<uint8_t> rawData;
    rawData.reserve(tileCount * 2);
    for (int i = 0; i < tileCount; ++i) {
         rawData.push_back(map.at(i).floor);
    }
    for (int i = 0; i < tileCount; ++i) {
        rawData.push_back(map.at(i).overlay);
    }
    //
    uLongf packedDataSize = compressBound(static_cast<uLong>(rawData.size()));
    std::vector<uint8_t> packedData(packedDataSize);
    //
    int result = compress2(packedData.data(), &packedDataSize, rawData.data(),
        static_cast<uLong>(rawData.size()), COMPRESSION_LEVEL);
    if (result != Z_OK)
        throw SerializerError("Data compression failed. Error code: " + result);
    //
    std::ofstream fout(path / "world_map.dat", std::ios::binary);
    fout.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
    fout.write(reinterpret_cast<const char*>(&packedDataSize), sizeof(packedDataSize));
    fout.write(reinterpret_cast<const char*>(packedData.data()), packedDataSize);
    logger.info() << "World map successfully saved.";
}

WorldMap serializer::loadMap(const std::filesystem::path& path) {
    std::ifstream fin(path / "world_map.dat", std::ios::binary);
    if (!fin.is_open())
        throw SerializerError("Failed to open file: " + (path / "world_map.dat").string());
    //
    TileCoord mapSize;
    uLongf packedDataSize = 0;
    fin.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
    fin.read(reinterpret_cast<char*>(&packedDataSize), sizeof(packedDataSize));
    //
    std::vector<uint8_t> packedData(packedDataSize);
    std::vector<uint8_t> rawData(mapSize.x * mapSize.y * 2);
    fin.read(reinterpret_cast<char*>(packedData.data()), packedDataSize);
    uLongf destLen = static_cast<uLongf>(rawData.size());
    //
    int result = uncompress(rawData.data(), &destLen, packedData.data(), packedDataSize);
    if (result != Z_OK)
        throw SerializerError("Data uncompression failed. Error code: " + std::to_string(result));
    //
    WorldMap map(mapSize);
    const int tileCount = mapSize.x* mapSize.y;
    for (int i = 0; i < tileCount; ++i) {
         map.at(i).floor = rawData[i];
    }
    for (int i = 0; i < tileCount; ++i) {
        map.at(i).overlay = rawData[i + tileCount];
    }
    logger.info() << "World map successfully load.";
    return map;
}
