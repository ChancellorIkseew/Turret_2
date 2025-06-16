#pragma once
#include <vector>
#include "engine/coords/tile_coord.hpp"
#include "engine/render/sprite.hpp"

class Building;
class Camera;

enum class TileType : uint8_t {
    WATER_DEEP,
    WATER_SHALLOW,
    SAND,
    GRASS,
    FOREST,
    MOUNTAIN_BASE,
    MOUNTAIN_PEAK
};

struct MapTile {
    TileType tileType;
    uint8_t resType;
    Building* building;
};

class World {
    std::vector<std::vector<MapTile>> terrain;
    Sprite sprite;
public:
    World(const TileCoord mapSize);
    void print();
    void draw(const Camera& camera);
};
