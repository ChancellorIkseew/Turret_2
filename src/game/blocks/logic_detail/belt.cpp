#include "game/blocks/block_types.hpp"
//
#include <cassert>
#include "game/blocks/block_map.hpp"

static inline Block* findNext(TileCoord tile, const BlockMap& map, BlockRot rotation) noexcept {
    constexpr TileCoord DIR_VECS[] = {
    TileCoord(0, -1), // up
    TileCoord(1, 0),  // right
    TileCoord(0, 1),  // down
    TileCoord(-1, 0)  // left
    };
    const TileCoord target = tile + DIR_VECS[rotation];
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
        if (itemX[i] > speed || itemX[i] < -speed) {
            const float step = (itemX[i] > 0.f) ? -speed * 2.f : +speed * 2.f;
            itemX[i] += step; 
            continue;
        }
        itemX[i] = 0.f;
        if (itemY[i] >= 1.0f) {
            needsRemove = true;
            continue;
        } 
        if (i > 0 && itemY[i] + ITEM_SPACE >= itemY[i - 1])
            continue;
        itemY[i] += speed;
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
    if (diff % 2 == 1) return minItem < 0.34f || minItem > 0.66f;
    return false;
}

void BeltBlock::accept(ItemPresetID item, BlockRot srcRot) {
    assert(len < CAPACITY);
    //
    if (srcRot == rotation) {
        itemID[len] = item;
        itemX[len] = 0.f;
        itemY[len] = 0.f;
        ++len;
        return;
    }
    
    if (srcRot == (rotation + 1) % 4) {
        itemID[len] = item;
        itemX[len] = -1.f;
        itemY[len] = 0.5f;
        ++len;
    }
    else {
        itemID[len] = item;
        itemX[len] = +1.f;
        itemY[len] = 0.5f;
        ++len;
    }
}
