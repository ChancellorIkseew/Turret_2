#include "game/blocks/block_types.hpp"
//
#include "blocks_common.hpp"
#include "engine/assets/presets.hpp"
#include "game/world/world_map.hpp"

FactoryBlock::FactoryBlock(const Presets& presets) {
	copper.item = presets.getItemID("item_copper");
	manganum.item = presets.getItemID("item_manganum");
	silicat.item = presets.getItemID("item_silicat");
	ceramite.item = presets.getItemID("item_ceramite");
}

bool FactoryBlock::canAccept(ItemPresetID item, BlockRot srcRot) {
	constexpr uint8_t MAX_STACK = 20;
	return (item == copper.item && copper.count <= MAX_STACK) ||
	(item == manganum.item && manganum.count <= MAX_STACK) ||
	(item == silicat.item && silicat.count <= MAX_STACK);
}

void FactoryBlock::accept(ItemPresetID item, BlockRot srcRot) {
	if (item == copper.item) ++copper.count;
	if (item == manganum.item) ++manganum.count;
	if (item == silicat.item) ++silicat.count;
}

void FactoryBlock::produce() {
	if (copper.count < 1 || manganum.count < 1 || silicat.count < 1)
		return;
	--copper.count;
	--manganum.count;
	--silicat.count;
	ceramite.count += 2;
}

void FactoryBlock::provide(TileCoord tile, const BlockMap& map) {
	throwItem(tile, size, map, ceramite, step);
}
