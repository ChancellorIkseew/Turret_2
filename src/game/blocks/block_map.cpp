#include "block_map.hpp"
//
#include <cassert>
#include "engine/assets/presets.hpp"
#include "make_block.hpp"

void BlockMap::place(TileCoord tile, TeamID teamID, std::unique_ptr<Block>& block) {
    demolish(tile);
    if (block->getType() == BlockType::turret) {
        TurretBlock* turretBlock = static_cast<TurretBlock*>(block.get());
        TurretPresetID preset = turretBlock->turretPreset;
        const AngleRad angleRad = static_cast<float>(turretBlock->defaultRotation) * t1::TAU;
        meta.getTurrets().addTurret(preset, t1::tileCenter(tile), t1::PI - angleRad, teamID, ShootingData(), 0);
    }
    if (block->getType() == BlockType::core)
        meta.setCorePosition(tile);
    if (block->getType() == BlockType::in_progress)
        meta.addBlockInProgress(tile);
    at(tile).place(teamID, block);
}

void BlockMap::demolish(TileCoord tile) {
    if (at(tile).type == BlockType::turret)
        meta.markForRemove(tile);
    if (at(tile).type == BlockType::core)
        meta.setCorePosition(std::nullopt);
    if (at(tile).type == BlockType::in_progress)
        meta.removeBlockInProgress(tile);
    at(tile).demolish();
}

void BlockMap::build(const TileCoord tile, const TeamID teamID, const int8_t buildSpeed, const Presets& presets) {
    assert(dynamic_cast<InProgress*>(at(tile).block.get()) && isInProgress(tile));
    //
    InProgress* blockInProgress = static_cast<InProgress*>(at(tile).block.get());
    blockInProgress->increeseProgress(buildSpeed);
    if (!blockInProgress->isProgressFull())
        return;
    if (blockInProgress->action == BPAction::demolish)
        demolish(tile);
    else /*BPAction::build*/ {
        const auto& preset = presets.getBlock(blockInProgress->presetID);
        std::unique_ptr<Block> block = makeBlock(blockInProgress->presetID, preset, blockInProgress->rotation);
        place(tile, teamID, block);
    }
}
