#include "block_map.hpp"
//
#include <cassert>
#include "engine/assets/presets.hpp"
#include "make_block.hpp"
#include "offset_table.hpp"

static inline TileCoord getMaster(const TileCoord tile, const BlockMap& map) {
    if (map.at(tile).type == BlockType::link)
        return static_cast<LinkBlock*>(map.at(tile).block.get())->masterTile;
    return tile;
}

bool BlockMap::canPlace(const TileCoord tile, const int size) const noexcept {
    if (tile.x < 0 || tile.y < 0 || tile.x + size > mapSize.x || tile.y + size > mapSize.y)
        return false;
    for (const TileCoord offset : t1::getTileOffsets(size)) {
        if (at(tile + offset).type != BlockType::air)
            return false;
    }
    return true;
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

    const TileCoord masterTile = getMaster(tile, *this);;
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
    const TileCoord masterTile = getMaster(tile, *this);
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

void BlockMap::startDemolition(const TileCoord tile) {
    const TileCoord masterTile = getMaster(tile, *this);
    BlockTile& blockTile = at(masterTile);
    //
    auto blockInProgress = std::make_unique<InProgress>();
    blockInProgress->action = BPAction::demolish;
    blockInProgress->rotation = at(masterTile).block->getRotation();
    blockInProgress->progress = 100;
    blockInProgress->presetID = at(masterTile).block->presetID;
    blockInProgress->size = at(masterTile).block->size;
    blockInProgress->health = 1;
    blockInProgress->textureRect = at(masterTile).block->textureRect;
    //
    Block* newMaster = blockInProgress.get();
    blockTile.type = BlockType::in_progress;
    blockTile.block = std::move(blockInProgress);

    for (const TileCoord offset : t1::getTileOffsets(newMaster->size)) {
        BlockTile& linkTile = at(masterTile + offset);
        assert(linkTile.type == BlockType::link);
        auto* link = static_cast<LinkBlock*>(linkTile.block.get());
        link->master = newMaster;
    }
}
