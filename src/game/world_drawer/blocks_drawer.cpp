#include "blocks_drawer.hpp"
//
#include "engine/assets/assets.hpp"
#include "engine/render/renderer.hpp"
#include "game/player/camera.hpp"
#include "game/blocks/block_map.hpp"

constexpr PixelCoord SHADOW_SIZE = PixelCoord(38.0f, 38.0f);
constexpr PixelCoord BLENDING_AREA = PixelCoord(3.0f, 3.0f);

void BlocksDrawer::draw(const BlockMap& blocks, const Assets& assets, const Camera& camera, Renderer& renderer) {
    const TileCoord start = camera.getBuildingsStartTile();
    const TileCoord end = camera.getEndTile();
    const Texture shadowTexture = assets.getAtlas().at("block_shadow");
    cashedTiles.clear();
    //
    for (int x = start.x; x < end.x; ++x) {
        for (int y = start.y; y < end.y; ++y) {
            if (blocks.at(x, y).type != BlockType::air)
                cashedTiles.emplace_back(x, y);
        }
    }
    //
    renderer.setColorModifier(0, 0, 0, 64);
    for (const auto& tile : cashedTiles) {
        renderer.drawFast(shadowTexture, t1::pixel(tile) - BLENDING_AREA, SHADOW_SIZE);
    }
    renderer.resetColorModifier();
    for (const auto& tile : cashedTiles) {
        blocks.at(tile).block->draw(renderer, tile);
    }
}
