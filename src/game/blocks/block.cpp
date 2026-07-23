#include "block.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/render/renderer.hpp"
#include "engine/settings/settings.hpp"
#include "game/blocks/schematic/schematic.hpp"
#include "game/world_drawer/blocks_drawer.hpp"

void Block::draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile) {
    renderer.draw(textureRect, t1::pixel(tile), t1::TILE_PC * static_cast<float>(size));
}

void LinkBlock::draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile) {
    if (Settings::gameplay.showDebugInfo)
        renderer.drawRect(t1::pixel(tile), t1::TILE_PC, { 0.f, 0.f }, 0.f, 0x00'FF'00'40);
}

void InProgress::draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile) {
    blockDrawer.addInProgress(tile);
}

void InProgress::drawProgress(Renderer& renderer, const Presets& presets, TileCoord tile) const {
    const int16_t buildTime = presets.getBlock(presetID).buildTime;
    const float ratio = std::clamp(static_cast<float>(progress) / static_cast<float>(buildTime), 0.f, 1.f);
    const uint32_t fillAmount = static_cast<uint32_t>(255.f * ratio);
    const BlockRot dstRotation = rotation != BlockRot::none ? rotation : BlockRot::up;
    //
    const uint32_t baseColor = action == BPAction::build ? cl::BEIGE_TR : cl::RED_TR;
    const uint32_t color = baseColor | fillAmount;
    Schematic::drawBlock(presets, renderer, tile, presetID, dstRotation, color, false);
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
