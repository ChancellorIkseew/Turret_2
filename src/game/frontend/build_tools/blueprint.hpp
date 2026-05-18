#pragma once
#include <unordered_map>
#include "engine/coords/math.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/engine.hpp"
#include "engine/render/renderer.hpp"
#include "game/blocks/block.hpp"

struct Blueprint {
    TileCoord tile;
    BlockPresetID presetID;
    BlockRot rotation;
    uint8_t progress;
};

struct TileHash {
    std::size_t operator()(const TileCoord& tile) const noexcept {
        return std::hash<int>()(tile.x) ^ (std::hash<int>()(tile.y) << 1);
    }
};

class Blueprints {
    static constexpr size_t MAX_ELEMENTS = 512;
    std::unordered_map<TileCoord, Blueprint, TileHash> blueprints;
public:

    void addOrReplace(const TileCoord tile, const BlockPresetID presetID, const BlockRot rotation) {
        if (blueprints.size() < MAX_ELEMENTS)
            blueprints.insert_or_assign(tile, Blueprint(tile, presetID, rotation));
    }

    Blueprint& getClosest(const TileCoord target) noexcept {
        int minSqrDistance = std::numeric_limits<int>::max();
        TileCoord best;
        for (const auto& [tile, _] : blueprints) {
            const int sqrDistance = t1::pow2i(tile.x - target.x) + t1::pow2i(tile.y - target.y);
            if (sqrDistance < minSqrDistance) {
                minSqrDistance = sqrDistance;
                best = tile;
            }
        }
        return blueprints.at(best);
    }

    void draw(Renderer& renderer, const Engine& engine) const {
        const float timeMs = static_cast<float>(engine.getMainWindow().getTime());
        const float modifier = std::sin(timeMs / 500.f) * 64.f;
        const uint8_t alpha = uint8_t(modifier) + 191; // 255 - 64
        renderer.setColorModifier(255, 255, 255, alpha);
        for (const auto& [tile, blueprint] : blueprints) {
            const BlockPreset& preset = engine.getAssets().getPresets().getBlock(blueprint.presetID);
            const PixelCoord position = t1::pixel(tile);
            const PixelCoord size = preset.visual.size;
            if (!preset.rotatable)
                renderer.drawFast(preset.visual.texture, position, size);
            else {
                const float angleRad = static_cast<float>(blueprint.rotation) * t1::PI_F / 2.f;
                const PixelCoord origin = size / 2.f;
                renderer.draw(preset.visual.texture, position + origin, size, origin, angleRad);
            }
        }
        renderer.resetColorModifier();
    }

    bool empty() const noexcept { return blueprints.empty(); }
    void removeIfExists(const TileCoord tile) noexcept { blueprints.erase(tile); }
};
