#pragma once
#include "map_saver.hpp"

class World;

class WorldSaver {
public:
    static void save(const World& world) {
        MapSaver::save(world);
    }
};
