#include "block_map.hpp"
//
#include <cassert>
#include "engine/assets/presets.hpp"
#include "game/frontend/build_tools/blueprint.hpp"
#include "make_block.hpp"
#include "offset_table.hpp"

bool BlockMap::canPlace(const TileCoord tile, const int size) const noexcept {
    if (tile.x < 0 || tile.y < 0 || tile.x + size > mapSize.x || tile.y + size > mapSize.y)
        return false;
    for (const TileCoord offset : t1::getTileOffsets(size)) {
        if (at(tile + offset).type != BlockType::air)
            return false;
    }
    return at(tile).type == BlockType::air;
}

void BlockMap::place(TileCoord tile, TeamID teamID, std::unique_ptr<Block>& block) {
    if (!canPlace(tile, block->size))
        return;

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

    TileCoord masterTile = tile;
    for (const TileCoord offset : t1::getTileOffsets(block->size)) {
        const TileCoord target = masterTile + offset;
        at(target).teamID = teamID;
        at(target).type = BlockType::link;
        at(target).block = std::make_unique<LinkBlock>(masterTile, block.get());
    }
    at(masterTile).place(teamID, block);
}

void BlockMap::demolish(TileCoord tile) {
    if (at(tile).type == BlockType::air)
        return;

    const TileCoord masterTile = getMaster(tile);
    if (at(masterTile).type == BlockType::turret)
        meta.markForRemove(masterTile);
    if (at(masterTile).type == BlockType::core)
        meta.setCorePosition(std::nullopt);
    if (at(masterTile).type == BlockType::in_progress)
        meta.removeBlockInProgress(masterTile);

    const int size = at(masterTile).block->size;
    for (const TileCoord offset : t1::getTileOffsets(size)) {
        at(masterTile + offset).demolish();
    }
    at(masterTile).demolish();
}

void BlockMap::build(const TileCoord tile, const TeamID teamID, const int8_t buildSpeed, const Presets& presets) {
    const TileCoord masterTile = getMaster(tile);
    assert(isInProgress(masterTile));
    //
    InProgress* blockInProgress = static_cast<InProgress*>(at(masterTile).block.get());
    blockInProgress->increeseProgress(buildSpeed);
    if (!blockInProgress->isProgressFull())
        return;
    if (blockInProgress->action == BPAction::demolish)
        demolish(masterTile);
    else /*BPAction::build*/ {
        const auto& preset = presets.getBlock(blockInProgress->presetID);
        std::unique_ptr<Block> block = makeBlock(blockInProgress->presetID, preset, blockInProgress->rotation);
        demolish(masterTile);
        place(masterTile, teamID, block);
    }
}

void BlockMap::applyBlueprint(const Blueprint& blueprint, const TeamID teamID, const Presets& presets) {
    std::unique_ptr<Block> block = std::make_unique<InProgress>();
    InProgress* blockInProgress = static_cast<InProgress*>(block.get());
    if (blueprint.action == BPAction::demolish) {
        blockInProgress->action = BPAction::demolish;
        blockInProgress->rotation = at(blueprint.tile).block->getRotation();
        blockInProgress->progress = 100;
        block->presetID = at(blueprint.tile).block->presetID;
        block->size = at(blueprint.tile).block->size;
        block->textureRect = at(blueprint.tile).block->textureRect;
        demolish(blueprint.tile);
    }
    else {
        blockInProgress->action = BPAction::build;
        blockInProgress->rotation = blueprint.rotation;
        blockInProgress->progress = 0;
        block->presetID = blueprint.presetID;
        block->size = presets.getBlock(blueprint.presetID).size;
        block->textureRect = presets.getBlock(blueprint.presetID).visual.textureRect;
    }
    block->health = 1;
    place(blueprint.tile, teamID, block);
}
