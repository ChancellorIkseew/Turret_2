#pragma once
#include <vector>
#include "config.hpp"
#include "engine/coords/tile_coord.hpp"
#include "engine/render/sprite.hpp"
#include "engine/coords/transforms.hpp"
#include "map_content.hpp"

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

struct MapTile {
    uint8_t floor = 0;
    uint8_t overlay = 0;
    Block* block = nullptr;
};

class WorldMap {
    ContentIndexes indexes;
    std::vector<MapTile> terrain;
    const TileCoord mapSize;
public:
    WorldMap(const TileCoord mapSize);
    void print();
    void placeFloor(const TileCoord tile, const uint8_t floorID);
    void placeOverlay(const TileCoord tile, const uint8_t overlayID);
    void placeBlock();

    t1_finline bool tileExists(const int tileX, const int tileY) const {
        return tileX >= 0 && tileX < mapSize.x &&
            tileY >= 0 && tileY < mapSize.y;
    }
    t1_finline bool tileExists(const TileCoord tile) const {
        return tileExists(tile.x, tile.y);
    }

    const ContentIndexes& getContentIndexes() const {
        return indexes;
    }

    t1_finline const MapTile& at(const int x, const int y) const noexcept { return terrain[x + y * mapSize.x]; }
    t1_finline const MapTile& at(const TileCoord tile)     const noexcept { return at(tile.x, tile.y); }
    t1_finline const MapTile& at(const int i)              const noexcept { return terrain[i]; }
    t1_finline       MapTile& at(const int x, const int y)       noexcept { return terrain[x + y * mapSize.x]; }
    t1_finline       MapTile& at(const TileCoord tile)           noexcept { return at(tile.x, tile.y); }
    t1_finline       MapTile& at(const int i)                    noexcept { return terrain[i]; }
    //
    TileCoord getSize() const noexcept { return mapSize; }
};
