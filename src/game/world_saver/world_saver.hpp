#pragma once
#include "engine/io/folders.hpp"
#include "map_saver.hpp"
#include <string>

class World;

class WorldSaver {
public:
    static void save(const World& world, const std::string& folder) {
        if (!io::folders::createOrCheckFolder(io::folders::SAVES))
            return;
        const std::filesystem::path path = io::folders::SAVES / folder;
        if (!io::folders::createOrCheckFolder(path))
            return;
        MapSaver::save(world, path);
    }
};
