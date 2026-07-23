#include "block_map.hpp"
//
#include <cassert>
#include "engine/assets/presets.hpp"
#include "game/blocks/schematic/schematic.hpp"
#include "game/common/inventory.hpp"
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
        const PixelCoord origin = t1::HALF_TILE_PC * static_cast<float>(block->size);
        meta.getTurrets().addTurret(preset, tile, t1::pixel(tile) + origin, t1::PI - angleRad, teamID, ShootingData(), 0);
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

void BlockMap::build(const TileCoord tile, const TeamID teamID, const int16_t buildSpeed, const Presets& presets, Inventory& inventory) {
    const TileCoord masterTile = getMaster(tile);
    assert(isInProgress(masterTile));

    InProgress* blockInProgress = static_cast<InProgress*>(at(masterTile).block.get());
    const auto& preset = presets.getBlock(blockInProgress->presetID);
    const int16_t totalTime = std::max<int16_t>(1, preset.buildTime);

    if (blockInProgress->action == BPAction::build) {
        const int16_t remaining = totalTime - std::min(blockInProgress->progress, totalTime);
        const int16_t maxStep = std::min(remaining, buildSpeed);

        bool canAdvance = true;
        for (const auto& ing : preset.recipe) {
            if (ing.itemID.asUint() == 0)
                break;

            const int16_t resourceNeeded = std::max<int16_t>(1, (ing.amount * maxStep) / totalTime);
            if (!inventory.has(ing.itemID, resourceNeeded)) {
                canAdvance = false;
                break;
            }
        }

        if (canAdvance) {
            for (const auto& ing : preset.recipe) {
                if (ing.itemID.asUint() == 0)
                    break;
                const int16_t resourceToWaste = std::max<int16_t>(1, (ing.amount * maxStep) / totalTime);
                inventory.waste(ing.itemID, resourceToWaste);
            }
            blockInProgress->increeseProgress(maxStep);
        }
    }
    else /* BPAction::demolish */ {
        const int16_t maxStep = std::min(blockInProgress->progress, buildSpeed);
        for (const auto& ing : preset.recipe) {
            if (ing.itemID.asUint() == 0)
                break;
            const int16_t resourceToReturn = std::max<int16_t>(1, (ing.amount * maxStep) / totalTime);
            inventory.add(ing.itemID, resourceToReturn);
        }

        blockInProgress->increeseProgress(maxStep);
    }

    if (!blockInProgress->isProgressFull(totalTime))
        return;
    if (blockInProgress->action == BPAction::demolish) {
        demolish(masterTile);
    }
    else {
        std::unique_ptr<Block> block = makeBlock(blockInProgress->presetID, preset, blockInProgress->rotation);
        demolish(masterTile);
        place(masterTile, teamID, block);
    }
}

void BlockMap::applyBlueprint(const Blueprint& blueprint, const TeamID teamID, const Presets& presets) {
    std::unique_ptr<Block> block = std::make_unique<InProgress>();
    InProgress* blockInProgress = static_cast<InProgress*>(block.get());
    const BlockPreset& preset = presets.getBlock(blueprint.presetID);
    if (blueprint.action == BPAction::demolish) {
        blockInProgress->action = BPAction::demolish;
        blockInProgress->rotation = at(blueprint.tile).block->getRotation();
        blockInProgress->progress = preset.buildTime;
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
        block->size = preset.size;
        block->textureRect = preset.visual.textureRect;
    }
    block->health = 1;
    place(blueprint.tile, teamID, block);
}

std::optional<InProgressAim> BlockMap::getClosestInProgress(const PixelCoord mobPosition) {
    const auto& inProgressList = meta.getBlocksInProgress();
    if (inProgressList.empty())
        return std::nullopt;

    std::optional<InProgressAim> closest;
    float minSqDistance = std::numeric_limits<float>::max();

    for (const TileCoord tile : inProgressList) {
        const BlockTile& blockTile = at(tile);
        if (!blockTile.block)
            continue;

        const PixelCoord halfSize = t1::HALF_TILE_PC * static_cast<float>(blockTile.block->size);
        const PixelCoord blockCenter = t1::pixel(tile) + halfSize;
        const float sqDistance = t1::squareDistance(mobPosition, blockCenter);

        if (sqDistance < minSqDistance) {
            minSqDistance = sqDistance;
            closest = InProgressAim(sqDistance, tile, blockCenter);
        }
    }

    return closest;
}
