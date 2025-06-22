#pragma once
#include <vector>
#include "engine/coords/tile_coord.hpp"

class Building;

enum class TileType : uint8_t {
    ROCK = 0,
    SNOW = 1,
    ICE = 2,
};

struct MapTile {
    TileType tileType;
    uint8_t resType;
    Building* building;
};

class World {
    std::vector<std::vector<MapTile>> terrain;
public:
    World(const TileCoord mapSize);
    void print();
    const std::vector<std::vector<MapTile>>& getMap() const { return terrain; }
};
