#include "game/blocks/block.hpp"
//
#include <cassert>
#include "game/blocks/block_map.hpp"
#include "engine/debug/logger.hpp"

static debug::Logger logger("t1_test");

// Down, Right, Up, Left. Y+ - Down, X+ - Right.
constexpr TileCoord DIR[] = { {0,1}, {1,0}, {0,-1}, {-1,0} };

static constexpr float approach(float current, float target, float step) noexcept {
    if (current < target) return std::min(current + step, target);
    else                  return std::max(current - step, target);
}

static inline Block* findNext(TileCoord tile, const BlockMap& map, BlockRot rotation) noexcept {
    const TileCoord target = tile + DIR[rotation];
    if (!map.contains(target) || map.at(target).type <= BlockType::wall)
        return nullptr;
    return map.at(target).block.get();
}

static inline void removeHead(BeltBlock& belt) {
    --belt.len;
    for (int i = 0; i < belt.len; ++i) {
        belt.itemID[i] = belt.itemID[i + 1];
        belt.itemY[i] = belt.itemY[i + 1];
        belt.itemX[i] = belt.itemX[i + 1];
    }
}

void BeltBlock::update(TileCoord tile, const BlockMap& map) {
    Block* next = findNext(tile, map, rotation);
    bool needsRemove = false;

    for (int i = 0; i < len; ++i) {
        if (itemY[i] >= 1.0f) {
            needsRemove = true;
            continue;
        } 
        if (i > 0 && itemY[i] + ITEM_SPACE >= itemY[i - 1])
            continue;
        itemY[i] += 0.01f;
    }

    if (len > 0)
        minItem = itemY[len - 1];  
    else
        minItem = ITEM_SPACE;

    if (needsRemove && next && next->canAccept(itemID[0], rotation)) {
        next->accept(itemID[0], rotation);
        removeHead(*this);
    }
}

bool BeltBlock::canAccept(ItemPresetID item, BlockRot srcRot) {
    if (len >= CAPACITY) return false;
    if (srcRot == rotation) return minItem >= ITEM_SPACE;
    const int8_t diff = std::abs(srcRot - rotation);
    if (diff % 2 == 1) return minItem > 0.7f;
    return false;
}

void BeltBlock::accept(ItemPresetID item, BlockRot srcRot) {
    assert(len + 1 >= CAPACITY);
    //
    itemID[len] = item;
    itemX[len] = 0.f;
    itemY[len] = 0.f;
    ++len;
    minItem = 0.f;
    /*
    if (srcRot == rotation) {
        itemX[len] = 0.f;
        itemY[len] = 0.f;
    }
    else {
        itemX[len] = +-0.5;
        itemY[len] = 0.5;
    }
    ++len;*/
}
