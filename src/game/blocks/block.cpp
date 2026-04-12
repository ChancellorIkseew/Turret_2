#include "block.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/render/renderer.hpp"

void Block::draw(const Renderer& renderer, TileCoord tile) {
    renderer.drawFast(texture, t1::pixel(tile), t1::TILE_PC);
}

static constexpr float dirX[] = { 0.0f, 1.0f, 0.0f, -1.0f }; // Down, Right, Up, Left
static constexpr float dirY[] = { 1.0f, 0.0f, -1.0f, 0.0f }; // Предполагая Y+ это Up

void BeltBlock::draw(const Renderer& renderer, TileCoord tile) {
    // works perfect (do not touch logic)
    AngleRad angleRad = static_cast<float>(rotation) * t1::PI_F * 0.5f;
    renderer.draw(texture, t1::tileCenter(tile), t1::TILE_PC, t1::TILE_PC / 2, angleRad);

    // Кэшируем векторы для текущего поворота
    float dx = dirX[rotation];
    float dy = dirY[rotation];

    // Перпендикуляр (px, py) — это просто поворот (dx, dy)
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
        renderer.drawFast(texture, itemPos - PixelCoord(8, 8), PixelCoord(16, 16));
        //TODO replace with blockDrawer.addItem(item[i], position);
    }
}
