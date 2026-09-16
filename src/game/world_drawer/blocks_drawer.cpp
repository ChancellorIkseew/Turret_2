#include "blocks_drawer.hpp"
//
#include "engine/assets/assets.hpp"
#include "engine/render/renderer.hpp"
#include "game/player/camera.hpp"
#include "game/blocks/block_map.hpp"

void BlocksDrawer::drawShadows(const BlockMap& blocks, const Camera& camera, Renderer& renderer) {
    const TileCoord start = camera.getBuildingsStartTile();
    const TileCoord end = camera.getEndTile();

    cashedTiles.clear();
    inProgress.clear();
    itemPositions.clear();
    itemPresetIDs.clear();
    //
    for (int x = start.x; x < end.x; ++x) {
        for (int y = start.y; y < end.y; ++y) {
            if (blocks.at(x, y).type != BlockType::air)
                cashedTiles.emplace_back(x, y);
        }
    }
    //
    for (const auto& tile : cashedTiles) {
        constexpr PixelCoord SHADOW_SIZE(64.f, 64.f);
        constexpr PixelCoord BLENDING_AREA(16.f, 16.f);
        renderer.draw(FULL_UV_RECT, t1::pixel(tile) - BLENDING_AREA, SHADOW_SIZE);
    }
}

void BlocksDrawer::draw(const BlockMap& blocks, const Presets& presets, Renderer& renderer) {
    for (const auto& tile : cashedTiles) {
        if (blocks.at(tile).type == BlockType::belt)
            blocks.at(tile).block->draw(*this, renderer, tile);
    }

    int i = 0;
    for (ItemPresetID itemPresetID : itemPresetIDs) {
        constexpr PixelCoord ITEM_SIZE(16.f, 16.f);
        TextureRect textureRect = presets.getItem(itemPresetID).textureRect;
        renderer.draw(textureRect, itemPositions[i], ITEM_SIZE);
        ++i;
    }

    for (const auto& tile : cashedTiles) {
        if (blocks.at(tile).type != BlockType::belt)
            blocks.at(tile).block->draw(*this, renderer, tile);
    }
}

void BlocksDrawer::drawInProgress(const BlockMap& blocks, const Presets& presets, Renderer& renderer) {
    for (const auto& tile : inProgress) {
        static_cast<InProgress*>(blocks.at(tile).block.get())->drawProgress(renderer, presets, tile);
    }
}
