#include "blocks_system.hpp"
//
#include "blocks.hpp"
#include "engine/assets/assets.hpp"
#include "engine/render/renderer.hpp"
#include "game/player/camera.hpp"

constexpr PixelCoord SHADOW_SIZE = PixelCoord(38.0f, 38.0f);
constexpr PixelCoord BLENDING_AREA = PixelCoord(3.0f, 3.0f);
constexpr PixelCoord ORIGIN = PixelCoord(32.0f, 32.0f);
constexpr double ANGLE = 0.0;

void blocks::drawBlocks(const Blocks& blocks, const Assets& assets, const Camera& camera, Renderer& renderer) {
    const TileCoord start = camera.getBuildingsStartTile();
    const TileCoord end = camera.getEndTile();

    const auto& presets = assets.getPresets();
    const auto& atlas = assets.getAtlas();
    Texture shadowTexture = atlas.at("block_shadow");

    renderer.setColorModifier(0, 0, 0, 64);
    for (int x = start.x; x < end.x; ++x) {
        for (int y = start.y; y < end.y; ++y) {
            if (blocks.preset[blocks.at(x, y)] != BLOCK_AIR)
                renderer.drawFast(shadowTexture, t1::pixel(x, y) - BLENDING_AREA, SHADOW_SIZE);
        }
    }
    renderer.resetColorModifier();

    for (int x = start.x; x < end.x; ++x) {
        for (int y = start.y; y < end.y; ++y) {
            if (blocks.preset[blocks.at(x, y)] == BLOCK_AIR)
                continue;
            const auto& visual = presets.getBlock(blocks.preset[blocks.at(x, y)]).visual;
            renderer.drawAnimated(visual.texture, t1::pixel(x, y), visual.size, ORIGIN, ANGLE, 0, visual.frameHeight);
        }
    }
}
