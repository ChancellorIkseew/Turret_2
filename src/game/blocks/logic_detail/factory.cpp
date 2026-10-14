#include "game/blocks/block_types.hpp"
//
#include "blocks_common.hpp"
#include "engine/assets/presets.hpp"
#include "game/world/world_map.hpp"

constexpr uint8_t MAX_STACK = 20;

FactoryBlock::FactoryBlock(const BlockPreset& preset) {
	for (const auto it : preset.inputItems) {
		inputs.push_back(ItemStack(it.itemID, 0));
	}
	output = ItemStack(preset.outputItem.itemID, 0);
}

bool FactoryBlock::canAccept(ItemPresetID item, BlockRot srcRot) {
	for (const auto it : inputs) {
		if (item == it.item && it.count < MAX_STACK)
			return true;
	}
	return false;
}

void FactoryBlock::accept(ItemPresetID item, BlockRot srcRot) {
	for (auto& it : inputs) {
		if (item == it.item) {
			++it.count;
			break;
		}
	}
}

void FactoryBlock::produce(const Presets& presets) {
	if (output.count >= MAX_STACK)
		return;
	const auto& recipe = presets.getBlock(Block::presetID).inputItems;
	for (size_t i = 0; i < recipe.size(); ++i) {
		if (inputs[i].count < recipe[i].amount)
			return;
	}
	for (size_t i = 0; i < recipe.size(); ++i) {
		inputs[i].count -= recipe[i].amount;
	}
	output.count += presets.getBlock(Block::presetID).outputItem.amount;
}

void FactoryBlock::provide(TileCoord tile, const BlockMap& map) {
	throwItem(tile, size, map, output, step);
}
