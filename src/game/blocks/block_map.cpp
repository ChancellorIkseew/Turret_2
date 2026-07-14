#include "block_map.hpp"
//
#include <cassert>
#include "engine/assets/presets.hpp"
#include "make_block.hpp"

void BlockMap::build(const TileCoord tile, const TeamID teamID, const int8_t buildSpeed, const Presets& presets) {
    assert(dynamic_cast<InProgress*>(at(tile).block.get()) && isInProgress(tile));
    //
    InProgress* blockInProgress = static_cast<InProgress*>(at(tile).block.get());
    blockInProgress->increeseProgress(buildSpeed);
    if (!blockInProgress->isProgressFull())
        return;
    if (blockInProgress->action == BPAction::demolish)
        demolish(tile);
    else /*BPAction::build*/ {
        const auto& preset = presets.getBlock(blockInProgress->presetID);
        std::unique_ptr<Block> block = makeBlock(blockInProgress->presetID, preset, blockInProgress->rotation);
        place(tile, teamID, block);
    }
}
