#include "blocks_system.hpp"
//
#include "blocks.hpp"
#include "engine/assets/presets.hpp"
#include "engine/render/renderer.hpp"
#include "game/player/camera.hpp"

constexpr PixelCoord ORIGIN = PixelCoord(32.0f, 32.0f);
constexpr double ANGLE = 0.0;

void blocks::drawBlocks(const Blocks& blocks, const Presets& presets, const Camera& camera, const Renderer& renderer) {
    const TileCoord start = camera.getBuildingsStartTile();
    const TileCoord end = camera.getEndTile();

    for (int x = start.x; x < end.x; ++x) {
        for (int y = start.y; y < end.y; ++y) {
            const auto& visual = presets.getBlock(blocks.preset[blocks.at(x, y)]).visual;
            renderer.drawAnimated(visual.texture, t1::pixel(x, y), visual.size, ORIGIN, ANGLE, 0, visual.frameHeight);
        }
    }
}
