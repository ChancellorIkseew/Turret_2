#pragma once

class World;

class MapSaver {
public:
    static void save(const World& world);
    static void load(const World& world);
};
