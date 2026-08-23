#pragma once
#include "game/blocks/block_map.hpp"

struct PerimeterStep {
    TileCoord tile;
    BlockRot moveDir;
};

inline constexpr PerimeterStep getPerimeterStep(const TileCoord masterTile, const int size, const int stepIndex) {
    const int side = stepIndex / size;   // 0: up, 1: right, 2: down, 3: left
    const int offset = stepIndex % size; // Offset on this side.

    switch (side) {
    case 0:
        return PerimeterStep{ TileCoord(masterTile.x + offset, masterTile.y - 1), BlockRot::up };
    case 1:
        return PerimeterStep{ TileCoord(masterTile.x + size, masterTile.y + offset), BlockRot::right };
    case 2:
        return PerimeterStep{ TileCoord(masterTile.x + (size - 1 - offset), masterTile.y + size), BlockRot::down };
    case 3:
        return PerimeterStep{ TileCoord(masterTile.x - 1, masterTile.y + (size - 1 - offset)), BlockRot::left };
    }
    return { masterTile, BlockRot::up };
}

inline void throwItem(const TileCoord masterTile, const int size, const BlockMap& map, ItemStack& stack, uint8_t& stepOffset) {
    const int totalSteps = size * 4;
    const int startOffset = stepOffset;

    for (int i = 0; i < totalSteps; ++i) {
        if (stack.count < 1)
            break;

        const int currentStep = (startOffset + i) % totalSteps;
        const PerimeterStep target = getPerimeterStep(masterTile, size, currentStep);
        if (!map.contains(target.tile))
            continue;

        const BlockTile& tile = map.at(target.tile);
        if (tile.type > BlockType::wall) {
            if (tile.block->canAccept(stack.item, target.moveDir)) {
                tile.block->accept(stack.item, target.moveDir);
                --stack.count;
                stepOffset = static_cast<uint8_t>((currentStep + 1) % totalSteps);
            }
        }
    }
}
