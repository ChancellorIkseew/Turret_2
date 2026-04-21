#include "game/blocks/block.hpp"
//
#include "engine/assets/presets.hpp"
#include "game/blocks/block_map.hpp"
#include "game/world/world_map.hpp"

#include "engine/debug/logger.hpp"

debug::Logger logger("drill");

static constexpr TileCoord DIR_VECS[] = {
    {0, 1}, // down
    {1, 0},  // right
    {0, -1},  // up
    {-1, 0}  // left
};

void DrillBlock::throwItem(TileCoord tile, const BlockMap& map, const WorldMap& terrain, const Presets& presets) {
    if (terrain.at(tile).ore == OrePresetID(0))
        return;
    ItemPresetID item = presets.getOre(OrePresetID(terrain.at(tile).ore)).item;

    for (int i = 0; i < 4; ++i) {
        //logger.error() << int(map.at(tile + DIR_VECS[i]).type);
        const TileCoord targetTile = tile + DIR_VECS[i];
        if (map.at(targetTile).type != BlockType::belt)
            continue;
        auto belt = static_cast<BeltBlock*>(map.at(targetTile).block.get());

        if (belt->canAccept(item, static_cast<BlockRot>(i))) {
            logger.warning() << "can";
            belt->accept(item, static_cast<BlockRot>(i));
            logger.warning() << int(belt->len);
        }
    }
}
