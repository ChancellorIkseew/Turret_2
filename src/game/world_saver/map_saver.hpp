#pragma once
#include <filesystem>

class World;

class MapSaver {
public:
    static void save(const World& world, const std::filesystem::path& path);
    static void load(World& world, const std::filesystem::path& path);
};
