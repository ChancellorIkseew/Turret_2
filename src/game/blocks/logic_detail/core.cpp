#include "game/blocks/block_types.hpp"
//
#include "game/common/team.hpp"

void CoreBlock::accept(ItemPresetID item, BlockRot srcRot) {
    if (team)
        team->getInventory().add(item, 1);
}
