#pragma once
#include <vector>
#include "engine/coords/tile_coord.hpp"
#include "engine/render/sprite.hpp"
#include "engine/coords/transforms.hpp"

class Block;

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
    Block* block;
};

class Block {
    Sprite shadow;
    Sprite base;
    const TileCoord tile;
public:
    Block(const std::string& shadow, const std::string& base, const TileCoord tile) :
        shadow(shadow), base(base), tile(tile) {
        this->shadow.setSize(PixelCoord(40, 40));
        this->base.setSize(PixelCoord(32, 32));
        this->shadow.setPosition(t1::pixel(tile) - PixelCoord(4, 4));
        this->base.setPosition(t1::pixel(tile));
    }
    void draw() {
        shadow.draw();
        base.draw();
    }
};


class World {
    std::vector<std::vector<MapTile>> terrain;
public:
    World(const TileCoord mapSize);
    void print();
    const std::vector<std::vector<MapTile>>& getMap() const { return terrain; }
    void placeTile(const TileCoord tile, const TileType tileType);
    void placeRes();
};
