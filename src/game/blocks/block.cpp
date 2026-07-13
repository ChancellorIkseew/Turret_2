#include "block.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/render/renderer.hpp"
#include "game/frontend/build_tools/blueprint.hpp"
#include "game/world_drawer/blocks_drawer.hpp"

void Block::draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile) {
    renderer.draw(textureRect, t1::pixel(tile), t1::TILE_PC);
}

void InProgress::draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile) {
    blockDrawer.addInProgress(tile);
}

void InProgress::drawInProgress(Renderer& renderer, const Presets& presets, TileCoord tile) const {
    BlockRot dstRotation = rotation != BlockRot::none ? rotation : BlockRot::up;


    const uint32_t baseColor = action == BPAction::build ? 0xFA'DC'86'00 : 0x84'34'34'00;
    const uint32_t color = baseColor + 0x4F + static_cast<uint32_t>(191.f * (float(progress) / 100.f));
    Blueprints::drawBlock(presets, renderer, tile, presetID, dstRotation, color, false);
}

static constexpr float dirX[] = { 0.0f, 1.0f, 0.0f, -1.0f }; // Up, Right, Down, Left
static constexpr float dirY[] = { -1.0f, 0.0f, 1.0f, 0.0f }; // Y+ - Down

void BeltBlock::draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile) {
    // works perfect (do not touch logic)
    AngleRad angleRad = static_cast<float>(rotation) * t1::TAU;
    renderer.draw(textureRect, t1::tileCenter(tile), t1::TILE_PC, t1::TILE_PC / 2, angleRad);

    float dx = dirX[rotation];
    float dy = dirY[rotation];

    // (px, py) - rotation (dx, dy)
    float px = -dy;
    float py = dx;

    PixelCoord center = t1::tileCenter(tile);

    for (int i = 0; i < len; i++) {
        float shiftForward = (itemY[i] - 0.5f) * t1::TILE_F;
        float shiftSide    = (itemX[i] * 0.5f) * t1::TILE_F;
        PixelCoord itemPos {
            center.x + (dx * shiftForward) + (px * shiftSide),
            center.y + (dy * shiftForward) + (py * shiftSide)
        };
        constexpr PixelCoord HALF_ITEM_SIZE(8.f, 8.f);
        blockDrawer.addItem(itemID[i], itemPos - HALF_ITEM_SIZE);
    }
}
