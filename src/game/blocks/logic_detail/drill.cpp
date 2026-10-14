#include "game/blocks/block_types.hpp"
//
#include "blocks_common.hpp"
#include "engine/assets/presets.hpp"
#include "game/world/world_map.hpp"

void DrillBlock::mine(TileCoord tile, const WorldMap& terrain, const Presets& presets) {
    constexpr int MAX_ITEMS = 10;
    if (terrain.at(tile).ore == OrePresetID(0) || inventory.count >= MAX_ITEMS)
        return;
    if (--restProductionTime > 0)
        return;
    restProductionTime = productionTime;
    inventory.item = presets.getOre(OrePresetID(terrain.at(tile).ore)).item;
    ++inventory.count;
}

void ImpactDrillBlock::mine(TileCoord tile, const WorldMap& terrain, const Presets& presets) {
    constexpr int MAX_ITEMS = 10;
    if (inventory.count >= MAX_ITEMS)
        return;
    if (--restProductionTime > 0)
        return;
    restProductionTime = productionTime;
    inventory.item = presets.getItemID("item_silicat");
    ++inventory.count;
}

void DrillBlock::provide(TileCoord tile, const BlockMap& map) {
    throwItem(tile, size, map, inventory, step);
}
void ImpactDrillBlock::provide(TileCoord tile, const BlockMap& map) {
    throwItem(tile, size, map, inventory, step);
}
