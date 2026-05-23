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
    uint8_t progress;

    bool operator==(const TileCoord& otherTile) const noexcept {
        return tile == otherTile;
    }
};

class Blueprints {
    static constexpr size_t MAX_ELEMENTS = 512;
    std::vector<Blueprint> blueprints;
    //
    constexpr auto findByTile(const TileCoord tile) noexcept {
        return std::find(blueprints.begin(), blueprints.end(), tile);
    }
public:
    void addOrReplace(const TileCoord tile, const BlockPresetID presetID, const BlockRot rotation) {
        auto it = findByTile(tile);
        if (it != blueprints.end()) // replace
            *it = Blueprint(tile, presetID, rotation);
        else if (blueprints.size() < MAX_ELEMENTS) // emplace
            blueprints.emplace_back(tile, presetID, rotation);
    }
    
    Blueprint* getClosest(const TileCoord target) noexcept {
        int minSqrDistance = std::numeric_limits<int>::max();
        Blueprint* closest = nullptr;
        for (auto& bp : blueprints) {
            const int sqrDistance = t1::pow2i(bp.tile.x - target.x) + t1::pow2i(bp.tile.y - target.y);
            if (sqrDistance < minSqrDistance) {
                minSqrDistance = sqrDistance;
                closest = &bp;
            }
        }
        return closest;
    }

    void draw(Renderer& renderer, const Engine& engine) const {
        const float timeMs = static_cast<float>(engine.getMainWindow().getTime());
        const float modifier = std::sin(timeMs / 500.f) * 64.f;
        const uint8_t alpha = uint8_t(modifier) + 191; // 255 - 64
        renderer.setColorModifier(255, 255, 255, alpha);
        for (const auto& blueprint : blueprints) {
            const BlockPreset& preset = engine.getAssets().getPresets().getBlock(blueprint.presetID);
            const PixelCoord position = t1::pixel(blueprint.tile);
            const PixelCoord size = preset.visual.size;
            if (blueprint.progress > 0) {
                constexpr PixelCoord OFFSET(1.f, 1.f);
                renderer.drawRect(0x00'FF'00'FF, position - OFFSET, size + OFFSET * 2.f);
                //TODO: update after renderer reimplementation
            }
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

    void removeIfExists(const TileCoord tile) noexcept {
        auto it = findByTile(tile);
        if (it == blueprints.end())
            return;
        if (it != blueprints.end() - 1)
            *it = std::move(blueprints.back());
        blueprints.pop_back();
    }
};
