#pragma once
#include <filesystem>

class WorldMap;

namespace serializer {
    void saveMap(const WorldMap& world, const std::filesystem::path& path);
    WorldMap loadMap(const std::filesystem::path& path);
}
