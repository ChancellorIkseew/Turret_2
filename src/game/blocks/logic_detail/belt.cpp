#include "game/blocks/block.hpp"
//
#include "game/blocks/block_map.hpp"

static constexpr float approach(float current, float target, float step) {
    if (current < target) return std::min(current + step, target);
    else                  return std::max(current - step, target);
}

bool BeltBlock::pass(uint8_t item, BlockRot srcRot) {
    return next && next->canAccept(item, srcRot);
}

void BeltBlock::moveItems() {
    minitem = 1.f;
    mid = 0;

    //skip updates if possible
    if (len == 0)
        return;

    const float nextMax = aligned ? 1.f - std::max(ITEM_SPACE - nextBelt->minitem, 0.f) : 1.f;
    const float moved = 1.f;

    for (int8_t i = len - 1; i >= 0; i--) {
        const float nextPos = (i == len - 1 ? 100.f : ys[i + 1]) - ITEM_SPACE;
        const float maxMove = std::clamp(nextPos - ys[i], 0.f, moved);

        ys[i] += maxMove;

        if (ys[i] > nextMax) ys[i] = nextMax;
        if (ys[i] > 0.5 && i > 0) mid = i - 1;
        xs[i] = approach(xs[i], 0, moved * 2);

        if (ys[i] >= 1.f && pass(ids[i], rotation)) {
            //align X position if passing forwards
            if (aligned)
                nextBelt->xs[nextBelt->lastInserted] = xs[i];
            //remove last item
            len = std::min(i, len);
        }
        else if (ys[i] < minitem)
            minitem = ys[i];
    }
}

BeltBlock* BeltBlock::findNext(TileCoord tile, const BlockMap& map) noexcept {
    // TODO: add rotation depend
    const BlockTile& blockTile = map.at(tile + TileCoord(1, 0));
    if (blockTile.type != BlockType::belt)
        return nullptr;
    return static_cast<BeltBlock*>(blockTile.block.get());
}

void BeltBlock::update(TileCoord tile, const BlockMap& map) {
    //next = findNext(tile, map);
    //moveItems();
}

static constexpr BlockRot getRelativeDir(TileCoord from, TileCoord to) {
    if (from.y > to.y) return up;
    if (from.x > to.x) return right;
    if (from.y < to.y) return down;
    if (from.x < to.x) return left;
    return none;
}

bool BeltBlock::canAccept(uint8_t item, BlockRot srcRot) {
    if (len >= CAPACITY) return false;
    if (srcRot == rotation) return true; // minitem >= ITEM_SPACE; //temp
    const int8_t diff = std::abs(srcRot - rotation);
    if (diff % 2 == 1) return minitem > 0.7f;
    return false;
}
