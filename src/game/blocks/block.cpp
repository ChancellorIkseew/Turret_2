#include "block.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/render/renderer.hpp"

void Block::draw(const Renderer& renderer, TileCoord tile) {
    renderer.drawFast(texture, t1::pixel(tile), t1::TILE_PC);
}

void BeltBlock::draw(const Renderer& renderer, TileCoord tile) {
    Block::draw(renderer, tile);
    //TODO: draw items
}
