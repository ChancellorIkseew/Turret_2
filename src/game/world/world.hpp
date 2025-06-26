#pragma once
#include <vector>
#include "engine/coords/tile_coord.hpp"
#include "engine/render/sprite.hpp"

class Structure;

enum class TileType : uint8_t {
    SNOW = 0,
    ICE = 1,
    WATER = 2,
    SOIL = 3,
    ROCK = 4,
    MAGMA = 5
};

struct MapTile {
    TileType tileType;
    uint8_t resType;
    Structure* structure;
};

class Structure {
protected:
    Sprite sprite;
public:
    virtual void draw() = 0;
};

class Tree : public Structure {
public:
    void draw() final {

    }
};


class World {
    std::vector<std::vector<MapTile>> terrain;
public:
    World(const TileCoord mapSize);
    void print();
    const std::vector<std::vector<MapTile>>& getMap() const { return terrain; }
};
