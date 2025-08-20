#pragma once
#include <filesystem>

struct WorldProperties;

class PropertiesSaver {
public:
    static void save(const WorldProperties& properties, const std::filesystem::path& path);
    static WorldProperties load(const std::filesystem::path& path);
};
