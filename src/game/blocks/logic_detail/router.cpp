#include "game/blocks/block_types.hpp"
//
#include "blocks_common.hpp"

void RouterBlock::provide(TileCoord tile, const BlockMap& map) {
    throwItem(tile, size, map, inventory, step);
}
