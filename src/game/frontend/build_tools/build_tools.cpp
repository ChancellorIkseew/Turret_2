#include "build_tools.hpp"

void BuildTools::drawOneBlock(Engine& engine, Renderer& renderer, const TileCoord tile, const TileData tileData) const {
    const PixelCoord position = t1::pixel(tile);
    uint8_t alpha = 255;
    if (content == JEIContent::only_blocks) { // maybe needs refactoring
        const float timeMs = static_cast<float>(engine.getMainWindow().getTimeMs());
        const float modifier = std::sin(timeMs / 500.f) * 64.f;
        alpha = uint8_t(modifier) + 191; // 255 - 64
    }
    //
    if (tileData.component == TileComponent::block) {
        const Presets& presets = engine.getAssets().getPresets();
        const BlockPreset& preset = presets.getBlock(BlockPresetID(tileData.id));
        const PixelCoord size = preset.visual.size;
        const bool canBuild = engine.getSession().getWorld().getBlocks().isAir(tile);
        const uint32_t color = canBuild ? 0xFF'FF'FF'00 + alpha : 0xB4'34'24'C8;

        const float angleRad = preset.rotatable ? static_cast<float>(rotation) * t1::PI_F * 0.5f : 0.f;
        const PixelCoord origin = size / 2.f;
        renderer.draw(preset.visual.textureRect, position + origin, size, origin, angleRad, color);
        if (preset.archetype == BlockType::turret) {
            const TurretPreset& turretPreset = presets.getTurret(preset.turret);
            const PixelCoord size = turretPreset.visual.size;
            renderer.draw(turretPreset.visual.textureRect, position + origin, size,
                turretPreset.visual.origin, angleRad + t1::PI_F, color);
        }
        return;
    }
    //
    if (tileData.component == TileComponent::overlay) {
        const OrePreset& preset = engine.getAssets().getPresets().getOre(OrePresetID(tileData.id));
        renderer.draw(preset.textureRect, position, t1::TILE_PC, PixelCoord(0.f, 0.f), 0.f);
        return;
    }
    //
    if (tileData.component == TileComponent::floor) {
        const std::string& textureName = engine.getAssets().getIndexes().getFloorByIndex(tileData.id);
        const TextureRect textureRect = engine.getAssets().getAtlas().at(textureName);
        constexpr PixelCoord BLENDING_AREA(4.f, 4.f);
        constexpr PixelCoord FLOOR_SIZE = t1::TILE_PC + BLENDING_AREA * 2.f;
        renderer.draw(textureRect, position - BLENDING_AREA, FLOOR_SIZE, PixelCoord(0.f, 0.f), 0.f);
        return;
    }
}
