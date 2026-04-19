#include "map_saver.hpp"
//
#include "engine/io/archive.hpp"
#include "engine/io/io.hpp"
#include "game/world/world_map.hpp"

void serializer::saveMap(const WorldMap& map, const std::filesystem::path& path) {
    std::string blob;
    archive::add(blob, map.getSize());
    archive::add(blob, map.getFloor());
    archive::add(blob, map.getOverlay());
    const std::string packedData = archive::pack(blob);
    io::writeFile(path / "world_map.dat", packedData);
}

WorldMap serializer::loadMap(const std::filesystem::path& path) {
    const std::string packedData = io::readFile(path / "world_map.dat");
    const std::string blob = archive::unpack(packedData);
    archive::Reader reader(blob);
    //
    TileCoord mapSize;
    std::vector<uint8_t> floor;
    std::vector<uint8_t> overlay;
    //
    reader.get(mapSize);
    reader.get(floor,   mapSize.x * mapSize.y);
    reader.get(overlay, mapSize.x * mapSize.y);
    return WorldMap(mapSize, floor, overlay);
}
