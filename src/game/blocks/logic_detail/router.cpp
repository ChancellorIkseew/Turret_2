#include "blocks_common.hpp"

void RouterBlock::provide(TileCoord tile, const BlockMap& map) {
    throwItem(tile, size, map, inventory, step);
}
