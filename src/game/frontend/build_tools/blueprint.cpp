#include "blueprint.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/engine.hpp"
#include "engine/render/renderer.hpp"

void Blueprints::draw(Renderer& renderer, const Engine& engine) const {
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
