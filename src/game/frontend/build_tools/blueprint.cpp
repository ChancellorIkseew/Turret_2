#include "blueprint.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/engine.hpp"
#include "engine/render/renderer.hpp"

static void drawBlock(const Presets& presets, Renderer& renderer, const TileCoord tile, const BlockPresetID presetID, const BlockRot rotation, const uint32_t color) {
    const BlockPreset& preset = presets.getBlock(presetID);
    const PixelCoord size = preset.visual.size;
    const PixelCoord position = t1::pixel(tile);

    const float angleRad = static_cast<float>(rotation) * t1::PI_F / 2.f;
    const PixelCoord origin = size / 2.f;
    renderer.draw(preset.visual.textureRect, position + origin, size, origin, angleRad, color);
    if (preset.archetype == BlockType::turret) {
        const TurretPreset& turret = presets.getTurret(preset.turret);
        const PixelCoord blockCenter = position + origin;
        renderer.draw(turret.visual.textureRect, blockCenter, turret.visual.size, turret.visual.origin, angleRad + t1::PI_F, color);
    }
}

void Blueprints::draw(Renderer& renderer, const Engine& engine) const {
    const float timeMs = static_cast<float>(engine.getMainWindow().getTimeMs());
    const float modifier = std::sin(timeMs / 500.f) * 64.f;
    const uint8_t alpha = uint8_t(modifier) + 191; // 255 - 64
    const uint32_t color = 0xFF'FF'FF'00 + alpha;
    for (const auto& blueprint : blueprints) {
        const Presets& presets = engine.getAssets().getPresets();
        const BlockPreset& preset = engine.getAssets().getPresets().getBlock(blueprint.presetID);
        const PixelCoord position = t1::pixel(blueprint.tile);
        const PixelCoord size = preset.visual.size;
        if (blueprint.progress > 0) {
            constexpr PixelCoord OFFSET(1.f, 1.f);
            //renderer.draw(0x00'FF'00'FF, position - OFFSET, size + OFFSET * 2.f);
            //TODO: update after renderer reimplementation
        }
        drawBlock(presets, renderer, blueprint.tile, blueprint.presetID, blueprint.rotation, color);
    }
}
