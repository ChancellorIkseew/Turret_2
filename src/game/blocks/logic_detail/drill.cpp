#include "game/blocks/block.hpp"
//
#include "game/blocks/block_map.hpp"

#include "engine/debug/logger.hpp"

debug::Logger logger("drill");

static constexpr TileCoord DIR_VECS[] = {
    {0, 1}, // down
    {1, 0},  // right
    {0, -1},  // up
    {-1, 0}  // left
};

void DrillBlock::throwItem(TileCoord tile, const BlockMap& map) {
    for (int i = 0; i < 4; ++i) {
        //logger.error() << int(map.at(tile + DIR_VECS[i]).type);
        const TileCoord targetTile = tile + DIR_VECS[i];
        if (map.at(targetTile).type != BlockType::belt)
            continue;
        auto belt = static_cast<BeltBlock*>(map.at(targetTile).block.get());

        bool b = belt->canAccept(1, static_cast<BlockRot>(i));

        logger.warning() << b;
    }
}
