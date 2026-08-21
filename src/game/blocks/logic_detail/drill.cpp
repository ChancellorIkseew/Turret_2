#include "blocks_common.hpp"
#include "engine/assets/presets.hpp"
#include "game/world/world_map.hpp"

void DrillBlock::mine(TileCoord tile, const WorldMap& terrain, const Presets& presets) {
    constexpr int MAX_ITEMS = 10;
    if (terrain.at(tile).ore == OrePresetID(0) || inventory.count >= MAX_ITEMS)
        return;
    inventory.item = presets.getOre(OrePresetID(terrain.at(tile).ore)).item;
    inventory.count = std::min(inventory.count + mineSpeed, MAX_ITEMS);
}

void DrillBlock::provide(TileCoord tile, const BlockMap& map) {
    throwItem(tile, size, map, inventory, step);
}
