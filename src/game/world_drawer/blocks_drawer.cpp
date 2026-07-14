#include "blocks_drawer.hpp"
//
#include "engine/assets/assets.hpp"
#include "engine/render/renderer.hpp"
#include "game/player/camera.hpp"
#include "game/blocks/block_map.hpp"

void BlocksDrawer::drawShadows(const BlockMap& blocks, const Camera& camera, Renderer& renderer) {
    const TileCoord start = camera.getBuildingsStartTile();
    const TileCoord end = camera.getEndTile();
    const TextureRect rect{ 0.f, 0.f, 1.f, 1.f };

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
        constexpr PixelCoord ORIGIN(0.f, 0.f);
        renderer.draw(rect, t1::pixel(tile) - BLENDING_AREA, SHADOW_SIZE, ORIGIN, 0.f, 0x00'00'00'FF);
    }
}

void BlocksDrawer::draw(const BlockMap& blocks, const Presets& presets, Renderer& renderer) {
    for (const auto& tile : cashedTiles) {
        blocks.at(tile).block->draw(*this, renderer, tile);
    }

    int i = 0;
    for (ItemPresetID itemPresetID : itemPresetIDs) {
        constexpr PixelCoord ITEM_SIZE(16.f, 16.f);
        TextureRect textureRect = presets.getItem(itemPresetID).textureRect;
        renderer.draw(textureRect, itemPositions[i], ITEM_SIZE);
        ++i;
    }
}

void BlocksDrawer::drawInProgress(const BlockMap& blocks, const Presets& presets, Renderer& renderer) {
    for (const auto& tile : inProgress) {
        static_cast<InProgress*>(blocks.at(tile).block.get())->drawInProgress(renderer, presets, tile);
    }
}
