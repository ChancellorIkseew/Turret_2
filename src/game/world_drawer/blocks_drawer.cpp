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
        blocks.at(tile).block->draw(*this, renderer, tile);
    }

    int i = 0;
    for (uint8_t itemID : itemIDs) {
        constexpr PixelCoord ITEM_SIZE(16.f, 16.f);
        Texture texture = assets.getAtlas().at("item_iron"); // TODO add item textures table
        renderer.drawFast(texture, itemPositions[i], ITEM_SIZE);
        ++i;
    }
}
