#include "build_tools.hpp"
//
#include "blueprint.hpp"
#include "engine/coords/transforms.hpp"
#include "game/world/world.hpp"

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
        const bool canBuild = engine.getSession().getWorld().getBlocks().isAir(tile);
        const uint32_t color = canBuild ? 0xFF'FF'FF'00 + alpha : 0xB4'34'24'C8;
        Blueprints::drawBlock(presets, renderer, tile, BlockPresetID(tileData.id), rotation, color);
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
