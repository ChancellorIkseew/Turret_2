#include "blueprint.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/render/renderer.hpp"

void Blueprints::drawRange(Renderer& renderer, const PixelCoord center, const float range) {
    constexpr PixelCoord CELL_SIZE(20.f, 3.f);
    constexpr float MAX_OFFSET = 8.f, CELL = 20.f;
    //
    const float perimeter = t1::PI * 2.f * range;
    const int cellsCount = static_cast<int>(perimeter / (CELL + MAX_OFFSET));
    const float angleStep = t1::PI * 2.f / static_cast<float>(cellsCount);
    for (int i = 0; i < cellsCount; ++i) {
        const float angle = static_cast<float>(i) * angleStep;
        const PixelCoord origin(CELL_SIZE.x / 2.f, CELL_SIZE.y / 2.f - range);
        renderer.drawRect(center, CELL_SIZE, origin, angle, 0x84'34'34'C8);
    }
}

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
        drawRange(renderer, t1::tileCenter(tile), turret.range);
    }
}

void Blueprints::drawGhosts(Renderer& renderer, const Presets& presets, const uint64_t timeMs) const {
    const float modifier = std::sin(static_cast<float>(timeMs) / 500.f) * 64.f;
    const uint8_t alpha = uint8_t(modifier) + 191; // 255 - 64
    const uint32_t color = 0xFF'FF'FF'00 + alpha;
    for (const auto& blueprint : blueprints) {
        if (blueprint.progress > 0)
            continue;
        drawBlock(presets, renderer, blueprint.tile, blueprint.presetID, blueprint.rotation, color);
    }
}

void Blueprints::drawInProgress(Renderer& renderer, const Presets& presets) const {
    for (const auto& blueprint : blueprints) {
        if (blueprint.progress < 1)
            continue;
        const uint32_t color = 0xFF'FF'FF'4F + uint32_t(191.f * (float(blueprint.progress) / 100.f));
        drawBlock(presets, renderer, blueprint.tile, blueprint.presetID, blueprint.rotation, color);
    }
}
