#include "block.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/render/renderer.hpp"

void Block::draw(const Renderer& renderer, TileCoord tile) {
    renderer.drawFast(texture, t1::pixel(tile), t1::TILE_PC);
}

void BeltBlock::draw(const Renderer& renderer, TileCoord tile) {
    AngleRad angleRad = static_cast<float>(rotation) * t1::PI_F * 0.5f;
    renderer.draw(texture, t1::tileCenter(tile), t1::TILE_PC, t1::TILE_PC / 2, angleRad);
    //TODO: draw items
}
