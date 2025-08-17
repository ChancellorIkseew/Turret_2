#pragma once
#include <filesystem>

class WorldMap;

class MapSaver {
public:
    static void save(const WorldMap& world, const std::filesystem::path& path);
    static WorldMap load(const std::filesystem::path& path);
};
