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
    /*PixelCoord itemPos;
    for (int i = 0; i < len; i++) {
        switch (rotation) {
        case up:
            itemPos.y = t1::pixelF(tile.y) + t1::TILE_F - ys[len];
            itemPos.x = t1::tileCenterF(tile.x) + xs[len];
            break;
        case down:
            itemPos.y = t1::pixelF(tile.y) + ys[len];
            itemPos.x = t1::tileCenterF(tile.x) + xs[len];
            break;
        case left:
            itemPos.x = t1::pixelF(tile.y) + t1::TILE_F - ys[len];
            itemPos.y = t1::tileCenterF(tile.x) + xs[len];
            //
            break;
        case right:
            itemPos.x = t1::pixelF(tile.y) + ys[len];
            itemPos.y = t1::tileCenterF(tile.x) + xs[len];
            break;
        default:
            return;
        }
        renderer.drawFast(texture, itemPos, PixelCoord(16, 16));
    }*/
}
