#include "game/blocks/block_map.hpp"

bool IntersectionBlock::canAccept(ItemPresetID item, BlockRot srcRot) {
    if (srcRot % 2 == BlockRot::down && vertical.item == ItemPresetID(0))
        return true;
    if (srcRot % 2 == BlockRot::right && horizontal.item == ItemPresetID(0))
        return true;
    return false;
}

void IntersectionBlock::accept(ItemPresetID item, BlockRot srcRot) {
    if (srcRot % 2 == BlockRot::down) {
        vertical.item = item;
        vertical.rotation = srcRot;
        return;
    }
    horizontal.item = item;
    horizontal.rotation = srcRot;
}

static inline void tryProvide(TileCoord target, const BlockMap& map, IntersectionBlock::RotatedItem& rotatedItem) {
    const BlockTile& blockTile = map.at(target);
    if (blockTile.type > BlockType::wall && blockTile.block->canAccept(rotatedItem.item, rotatedItem.rotation)) {
        blockTile.block->accept(rotatedItem.item, rotatedItem.rotation);
        rotatedItem.item = ItemPresetID(0);
        rotatedItem.rotation = BlockRot::none;
    }
}

void IntersectionBlock::provide(TileCoord tile, const BlockMap& map) {
    int x = tile.x;
    int y = tile.y;
    if (vertical.item != ItemPresetID(0)) {
        if (vertical.rotation == BlockRot::up)
            tryProvide(TileCoord(x, y - 1), map, vertical);
        else
            tryProvide(TileCoord(x, y + 1), map, vertical);
    }
    if (horizontal.item != ItemPresetID(0)) {
        if (horizontal.rotation == BlockRot::left)
            tryProvide(TileCoord(x - 1, y), map, horizontal);
        else
            tryProvide(TileCoord(x + 1, y), map, horizontal);
    }
}
