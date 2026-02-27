#pragma once
#include <vector>
#include "block_archetype.hpp"
#include "engine/assets/preset_defs.hpp"
#include "game/physics/physics_base.hpp"

constexpr BlockPresetID BLOCK_AIR = BlockPresetID(0);
constexpr BlockPresetID BLOCK_AUXILARY = BlockPresetID(1);

struct CommonBlockSoA {
    std::vector<BlockArchetype> archetype;
    std::vector<uint32_t> specializedSoaIndex;
    std::vector<BlockPresetID> preset;
    std::vector<Health> health;
    std::vector<TeamID> teamID;
};

struct TurretSoA {
    std::vector<PixelCoord> position;
    std::vector<AngleRad> angle;
    std::vector<TurretPresetID> preset;
    std::vector<TeamID> teamID;
    std::vector<ShootingData> shootingData;
    std::vector<TickCount> restReloadTime;
    std::vector<uint8_t> currentBarrel;
    std::vector<AngleRad> turretAngle;
    size_t turretCount = 0;
};

struct DrillSoA {

};

struct ConveyorSoA {

};

class BlockManager {
    TileCoord mapSize;
    CommonBlockSoA commonSoa;
    TurretSoA turretSoa;
public:
    BlockManager(const TileCoord mapSize);
    void removeBlock(const TileCoord tile);
    void addBlock(
        const TileCoord tile,
        const BlockArchetype archetype,
        const BlockPresetID preset,
        const Health health,
        const TeamID teamID);
    //
    t1_finline int at(const int x, const int y) const noexcept { return x + y * mapSize.x; }
    t1_finline int at(const TileCoord tile)     const noexcept { return at(tile.x, tile.y); }
    TileCoord getSize() const noexcept { return mapSize; }
    const CommonBlockSoA& getCommonSoa() const noexcept { return commonSoa; }
    const TurretSoA& getTurretSoa() const noexcept { return turretSoa; }
    CommonBlockSoA& getCommonSoa() noexcept { return commonSoa; }
    TurretSoA& getTurretSoa() noexcept { return turretSoa; }
    //
    t1_finline bool blockExists(const int tileX, const int tileY) const noexcept {
        return tileX >= 0 && tileX < mapSize.x &&
               tileY >= 0 && tileY < mapSize.y;
    }
    t1_finline bool blockExists(const TileCoord tile) const noexcept {
        return blockExists(tile.x, tile.y);
    }
    t1_finline bool isVoidBlock(const TileCoord tile) const noexcept {
        return blockExists(tile.x, tile.y) && commonSoa.archetype[at(tile)] == BlockArchetype::air;
    }
    t1_finline bool isFilledBlock(const TileCoord tile) const noexcept {
        return blockExists(tile.x, tile.y) && commonSoa.archetype[at(tile)] != BlockArchetype::air;
    }
};
