#include "blueprint.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/engine.hpp"
#include "engine/render/renderer.hpp"

void Blueprints::drawBlock(const Presets& presets, Renderer& renderer, const TileCoord tile,
    const BlockPresetID presetID, const BlockRot rotation, const uint32_t color) {
    const BlockPreset& preset = presets.getBlock(presetID);
    const PixelCoord size = preset.visual.size;
    const PixelCoord position = t1::pixel(tile);

    const float angleRad = preset.rotatable ? static_cast<float>(rotation) * t1::TAU : 0.f;
    const PixelCoord origin = size / 2.f;
    renderer.draw(preset.visual.textureRect, position + origin, size, origin, angleRad, color);
    if (preset.archetype == BlockType::turret) {
        const TurretPreset& turret = presets.getTurret(preset.turret);
        const PixelCoord blockCenter = position + origin;
        renderer.draw(turret.visual.textureRect, blockCenter, turret.visual.size, turret.visual.origin, angleRad, color);
    }
}

void Blueprints::drawGhosts(Renderer& renderer, const Engine& engine) const {
    const float timeMs = static_cast<float>(engine.getMainWindow().getTimeMs());
    const float modifier = std::sin(timeMs / 500.f) * 64.f;
    const uint8_t alpha = uint8_t(modifier) + 191; // 255 - 64
    const uint32_t color = 0xFF'FF'FF'00 + alpha;
    for (const auto& blueprint : blueprints) {
        if (blueprint.progress > 0)
            continue;
        const Presets& presets = engine.getAssets().getPresets();
        const BlockPreset& preset = engine.getAssets().getPresets().getBlock(blueprint.presetID);
        const PixelCoord position = t1::pixel(blueprint.tile);
        const PixelCoord size = preset.visual.size;
        drawBlock(presets, renderer, blueprint.tile, blueprint.presetID, blueprint.rotation, color);
    }
}

void Blueprints::drawInProgress(Renderer& renderer, const Engine& engine) const {
    for (const auto& blueprint : blueprints) {
        if (blueprint.progress < 1)
            continue;
        const Presets& presets = engine.getAssets().getPresets();
        const BlockPreset& preset = engine.getAssets().getPresets().getBlock(blueprint.presetID);
        const PixelCoord position = t1::pixel(blueprint.tile);
        const PixelCoord size = preset.visual.size;
        const uint32_t color = 0xFF'FF'FF'4F + uint32_t(191.f * (float(blueprint.progress) / 100.f));
        drawBlock(presets, renderer, blueprint.tile, blueprint.presetID, blueprint.rotation, color);
    }
}
