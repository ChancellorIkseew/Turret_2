#include "blocks_system.hpp"
//
#include "block_map.hpp"
#include "engine/assets/assets.hpp"
#include "engine/render/renderer.hpp"
#include "game/player/camera.hpp"

constexpr PixelCoord SHADOW_SIZE = PixelCoord(38.0f, 38.0f);
constexpr PixelCoord BLENDING_AREA = PixelCoord(3.0f, 3.0f);
constexpr PixelCoord ORIGIN = PixelCoord(32.0f, 32.0f);
constexpr double ANGLE = 0.0;

void blocks::drawBlocks(const BlockMap& blocks, const Assets& assets, const Camera& camera, Renderer& renderer) {
    const TileCoord start = camera.getBuildingsStartTile();
    const TileCoord end = camera.getEndTile();

    const auto& presets = assets.getPresets();
    const auto& atlas = assets.getAtlas();
    Texture shadowTexture = atlas.at("block_shadow");

    static std::vector<TileCoord> cashedTiles;
    cashedTiles.clear();

    renderer.setColorModifier(0, 0, 0, 64);
    for (int x = start.x; x < end.x; ++x) {
        for (int y = start.y; y < end.y; ++y) {
            if (blocks.at(x, y).type == BlockType::air)
                continue;
            renderer.drawFast(shadowTexture, t1::pixel(x, y) - BLENDING_AREA, SHADOW_SIZE);
            cashedTiles.emplace_back(x, y);
        }
    }
    renderer.resetColorModifier();
    for (auto& tile : cashedTiles) {
        blocks.at(tile).block->draw(renderer, tile);
    }
    //renderer.drawAnimated(visual.texture, t1::pixel(x, y), visual.size, ORIGIN, ANGLE, 0, visual.frameHeight);
}
