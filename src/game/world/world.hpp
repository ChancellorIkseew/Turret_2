#pragma once
#include <vector>
#include <unordered_map>
#include "config.hpp"
#include "engine/coords/tile_coord.hpp"
#include "engine/render/sprite.hpp"
#include "engine/coords/transforms.hpp"
#include "game/physics/team/teams_pool.hpp"

struct ElementRegistry {
    std::unordered_map<uint8_t, std::string> floorTypes;
    std::unordered_map<uint8_t, std::string> overlayTypes;
    std::unordered_map<uint16_t, std::string> blockTypes;
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

struct MapTile {
    uint8_t floor = 0;
    uint8_t overlay = 0;
    Block* block = nullptr;
};

class World {
    ElementRegistry registry;
    std::vector<std::vector<MapTile>> terrain;
    const TileCoord mapSize;
    TeamsPool teams;
public:
    World(const TileCoord mapSize);
    void print();
    const std::vector<std::vector<MapTile>>& getMap() const { return terrain; }
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

    const ElementRegistry& getContent() const {
        return registry;
    }

    const TeamsPool& getTeams() const { return teams; }
    TeamsPool& getTeams() { return teams; }
};
