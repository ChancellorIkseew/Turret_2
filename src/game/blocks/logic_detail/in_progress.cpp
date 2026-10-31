#include "game/blocks/block_types.hpp"
//
#include "game/blocks/block_map.hpp"

void InProgress::onPlace(BlocksMeta& meta, TileCoord tile, TeamID teamID, BlockRot rotation) {
    meta.removeBlockInProgress(tile);
}

void InProgress::onDemolish(BlocksMeta& meta, TileCoord tile) {
    meta.addBlockInProgress(tile);
}
