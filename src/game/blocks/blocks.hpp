#pragma once
#include <vector>
#include "engine/assets/preset_defs.hpp"
#include "game/physics/physics_base.hpp"

constexpr BlockPresetID BLOCK_AIR = BlockPresetID(0);
constexpr BlockPresetID BLOCK_AUXILARY = BlockPresetID(1);

class Blocks {
public:
    std::vector<BlockPresetID> preset;
    std::vector<Health> health;
    std::vector<TeamID> teamID;
    const TileCoord mapSize;
public:
    Blocks(const TileCoord mapSize) : mapSize(mapSize) {
        const size_t tileCount = static_cast<size_t>(mapSize.x * mapSize.y);
        preset.reserve(tileCount);
        health.reserve(tileCount);
        teamID.reserve(tileCount);
        for (size_t i = 0; i < tileCount; ++i) {
            preset.push_back(BLOCK_AIR);
            health.push_back(100);
            teamID.push_back(0);
        }
    }

    void placeBlock(const TileCoord tile, const BlockPresetID presetID) {
        if (!isVoidBlock(tile))
            return;
        preset[at(tile)] = presetID;
    }

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
        return blockExists(tile.x, tile.y) && preset[at(tile)].presetID != BLOCK_AIR.asUint();
    }

    t1_finline int at(const int x, const int y) const noexcept { return x + y * mapSize.x; }
    t1_finline int at(const TileCoord tile)     const noexcept { return at(tile.x, tile.y); }
    TileCoord getSize() const noexcept { return mapSize; }
};
