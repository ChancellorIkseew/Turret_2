#pragma once

#include <vector>
#include "game/physics/physics_base.hpp"


using BlockPresetID = uint16_t;

class Blocks {
public:
    std::vector<BlockPresetID> preset;
    std::vector<Health> health;
    std::vector<TeamID> teamID;
    const TileCoord mapSize;
public:

    t1_finline bool blockExists(const int tileX, const int tileY) const noexcept {
        return tileX >= 0 && tileX < mapSize.x &&
            tileY >= 0 && tileY < mapSize.y;
    }
    t1_finline bool blockExists(const TileCoord tile) const noexcept {
        return blockExists(tile.x, tile.y);
    }
    t1_finline bool isVoidBlock(const TileCoord tile) const noexcept {
        return blockExists(tile.x, tile.y); //expand
    }
    t1_finline bool isFilledBlock(const TileCoord tile) const noexcept {
        return blockExists(tile.x, tile.y); //expand
    }

    t1_finline int at(const int x, const int y) const noexcept { return x + y * mapSize.x; }
    t1_finline int at(const TileCoord tile)     const noexcept { return at(tile.x, tile.y); }
    TileCoord getSize() const noexcept { return mapSize; }
};
