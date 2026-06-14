#include "blocks_drawer.hpp"
//
#include "engine/assets/assets.hpp"
#include "engine/render/renderer.hpp"
#include "game/player/camera.hpp"
#include "game/blocks/block_map.hpp"

void BlocksDrawer::draw(const BlockMap& blocks, const Assets& assets, const Camera& camera, Renderer& renderer) {
    const TileCoord start = camera.getBuildingsStartTile();
    const TileCoord end = camera.getEndTile();
    const TextureRect shadowTextureRect = assets.getAtlas().at("block_shadow");
    cashedTiles.clear();
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
        constexpr PixelCoord SHADOW_SIZE(38.f, 38.f);
        constexpr PixelCoord BLENDING_AREA(3.f, 3.f);
        constexpr PixelCoord ORIGIN(0.f, 0.f);
        renderer.draw(shadowTextureRect, t1::pixel(tile) - BLENDING_AREA, SHADOW_SIZE, ORIGIN, 0.f, 0x00'00'00'40);
    }
    for (const auto& tile : cashedTiles) {
        blocks.at(tile).block->draw(*this, renderer, tile);
    }

    int i = 0;
    for (ItemPresetID itemPresetID : itemPresetIDs) {
        constexpr PixelCoord ITEM_SIZE(16.f, 16.f);
        TextureRect textureRect = assets.getPresets().getItem(itemPresetID).textureRect;
        renderer.draw(textureRect, itemPositions[i], ITEM_SIZE);
        ++i;
    }
}
