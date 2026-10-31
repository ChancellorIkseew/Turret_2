#include "game/blocks/block_types.hpp"
//
#include "game/blocks/block_map.hpp"
#include "game/common/team.hpp"

void CoreBlock::accept(ItemPresetID item, BlockRot srcRot) {
    if (team)
        team->getInventory().add(item, 1);
}

void CoreBlock::onPlace(BlocksMeta& meta, TileCoord tile, TeamID teamID, BlockRot rotation) {
    meta.addCore(tile);
}

void CoreBlock::onDemolish(BlocksMeta& meta, TileCoord tile) {
    meta.removeCore(tile);
}
