#pragma once
#include <vector>
#include "engine/coords/math.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/engine.hpp"
#include "engine/render/renderer.hpp"
#include "game/blocks/block.hpp"

struct Blueprint {
    TileCoord tile;
    BlockPresetID presetID;
    BlockRot rotation;
};

class Blueprints {
    std::vector<Blueprint> blueprints;
public:

    void add(const TileCoord tile, const BlockPresetID presetID, const BlockRot rotation) {
        blueprints.emplace_back(tile, presetID, rotation);
    }

    TileCoord getClosest(const TileCoord target) const {
        int minSqrDistance = 100000; // random max value
        TileCoord result;
        for (const auto& [tile, _, _] : blueprints) {
            const int sqrDistance = t1::pow2i(tile.x - target.x) + t1::pow2i(tile.y - target.y);
            if (sqrDistance < minSqrDistance) {
                minSqrDistance = sqrDistance;
                result = tile;
            }
        }
        return result;
    }

    void draw(Renderer& renderer, const Engine& engine) {
        const float timeMs = static_cast<float>(engine.getMainWindow().getTime());
        const float modifier = std::sin(timeMs / 500.f) * 64.f;
        const uint8_t alpha = uint8_t(modifier) + 191; // 255 - 64
        renderer.setColorModifier(255, 255, 255, alpha);
        for (const auto& [tile, presetID, rotation] : blueprints) {
            const BlockPreset& preset = engine.getAssets().getPresets().getBlock(presetID);
            const PixelCoord position = t1::pixel(tile);
            const PixelCoord size = preset.visual.size;
            if (!preset.rotatable)
                renderer.drawFast(preset.visual.texture, position, size);
            else {
                const float angleRad = static_cast<float>(rotation) * t1::PI_F / 2.f;
                const PixelCoord origin = size / 2.f;
                renderer.draw(preset.visual.texture, position + origin, size, origin, angleRad);
            }
        }
        renderer.resetColorModifier();
    }
};
