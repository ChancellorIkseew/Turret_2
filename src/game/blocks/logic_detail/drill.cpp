#include "game/blocks/block.hpp"
//
#include "engine/assets/presets.hpp"
#include "game/blocks/block_map.hpp"
#include "game/world/world_map.hpp"

void DrillBlock::mine(TileCoord tile, const WorldMap& terrain, const Presets& presets) {
    constexpr int MAX_ITEMS = 10;
    if (terrain.at(tile).ore == OrePresetID(0) || inventory.count >= MAX_ITEMS)
        return;
    inventory.item = presets.getOre(OrePresetID(terrain.at(tile).ore)).item;
    inventory.count = std::min(inventory.count + mineSpeed, MAX_ITEMS);
}

void DrillBlock::throwItem(TileCoord tile, const BlockMap& map) {
    for (int i = 0; i < 4; ++i) {
        if (inventory.count < 1)
            return;
        const TileCoord targetTile = tile + DIR_VECS[i];
        const BlockTile& blockTile = map.at(targetTile);
        if (blockTile.type > BlockType::wall) {
            if (blockTile.block->canAccept(inventory.item, static_cast<BlockRot>(i))) {
                blockTile.block->accept(inventory.item, static_cast<BlockRot>(i));
                --inventory.count;
            } 
        }
    }
}
