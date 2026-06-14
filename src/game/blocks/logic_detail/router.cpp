#include "game/blocks/block_map.hpp"

void RouterBlock::provide(TileCoord tile, const BlockMap& map) {
    if (inventory.count == 0)
        return;
    for (int i = 0; i < 4; ++i) {
        const TileCoord target = tile + DIR_VECS[i];
        if (map.at(target).type > BlockType::wall) {
            auto block = static_cast<Block*>(map.at(target).block.get());
            if (block->canAccept(inventory.item, static_cast<BlockRot>(i))) {
                block->accept(inventory.item, static_cast<BlockRot>(i));
                inventory.count = 0;
                return;
            }
        }
    }
}
