#include "block_manager.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/assets/presets.hpp"

BlockManager::BlockManager(const TileCoord mapSize) : mapSize(mapSize) {
    const int blockCount = mapSize.x * mapSize.y;
    commonSoa.archetype.assign(blockCount, BlockArchetype::air);
    commonSoa.specializedSoaIndex.assign(blockCount, 0);
    commonSoa.preset.assign(blockCount, BLOCK_AIR);
    commonSoa.health.assign(blockCount, 0);
    commonSoa.teamID.assign(blockCount, 0);
}

static uint32_t addTurret(const Presets& presets, const BlockPresetID preset,
    TurretSoA& soa, const TileCoord tile, const TeamID teamID) {

    ShootingData sData(ShootingAI::basic, false, PixelCoord(0, 0));
    soa.position.push_back(t1::tileCenter(tile));
    soa.angle.push_back(0.0f);
    soa.preset.push_back(presets.getBlock(preset).turret);
    soa.teamID.push_back(teamID);
    soa.shootingData.push_back(sData);
    soa.restReloadTime.push_back(0);
    soa.currentBarrel.push_back(0);
    soa.turretAngle.push_back(0.0f);
    return static_cast<uint32_t>(++soa.turretCount);
}

static void removeTurret(const uint32_t index, TurretSoA& soa) {
    const uint32_t last = static_cast<uint32_t>(soa.turretCount);

    if (index != last) {
        soa.position[index] = std::move(soa.position[last]);
        soa.angle[index] = std::move(soa.angle[last]);
        soa.preset[index] = std::move(soa.preset[last]);
        soa.teamID[index] = std::move(soa.teamID[last]);
        soa.shootingData[index] = std::move(soa.shootingData[last]);
        soa.restReloadTime[index] = std::move(soa.restReloadTime[last]);
        soa.currentBarrel[index] = std::move(soa.currentBarrel[last]);
        soa.turretAngle[index] = std::move(soa.turretAngle[last]);
    }

    soa.position.pop_back();
    soa.angle.pop_back();
    soa.preset.pop_back();
    soa.teamID.pop_back();
    soa.shootingData.pop_back();
    soa.restReloadTime.pop_back();
    soa.currentBarrel.pop_back();
    soa.turretAngle.pop_back();
}

void BlockManager::addBlock(
    const Presets& presets,
    const TileCoord tile,
    const BlockArchetype archetype,
    const BlockPresetID preset,
    const Health health,
    const TeamID teamID) {
    const int index = at(tile);
    if (commonSoa.archetype[index] != BlockArchetype::air)
        return; // TODO: isAirBlock()

    uint32_t specializedSoaIndex = 0;
    if (archetype == BlockArchetype::turret)
        specializedSoaIndex = addTurret(presets, preset, turretSoa, tile, teamID);

    commonSoa.archetype[index] = archetype;
    commonSoa.specializedSoaIndex[index] = specializedSoaIndex;//?
    commonSoa.preset[index] = preset;
    commonSoa.health[index] = 0;
    commonSoa.teamID[index] = 0;
}

void BlockManager::removeBlock(const TileCoord tile) {
    const int index = at(tile);

    if (commonSoa.archetype[index] == BlockArchetype::turret)
        removeTurret(index, turretSoa);

    commonSoa.archetype[index] = BlockArchetype::air;
    commonSoa.specializedSoaIndex[index] = 0;//?
    commonSoa.preset[index] = BLOCK_AIR;
    commonSoa.health[index] = 0;
    commonSoa.teamID[index] = 0;
}
