#include "schematic.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/render/renderer.hpp"

static void drawBlockFrame(Renderer& renderer, const TileCoord tile, const int size, const uint32_t color) {
    const PixelCoord position = t1::pixel(tile);
    const float pixelSize = t1::pixelF(size);
    //
    renderer.drawRect(position, PixelCoord(12, 4), PixelCoord(0, 0), 0.f, color);
    renderer.drawRect(position, PixelCoord(4, 12), PixelCoord(0, 0), 0.f, color);
    //
    const PixelCoord topRight(position.x + pixelSize, position.y);
    renderer.drawRect(topRight, PixelCoord(12, 4), PixelCoord(12, 0), 0.f, color);
    renderer.drawRect(topRight, PixelCoord(4, 12), PixelCoord(4, 0), 0.f, color);
    //
    const PixelCoord bottomLeft(position.x, position.y + pixelSize);
    renderer.drawRect(bottomLeft, PixelCoord(12, 4), PixelCoord(0, 4), 0.f, color);
    renderer.drawRect(bottomLeft, PixelCoord(4, 12), PixelCoord(0, 12), 0.f, color);
    //
    const PixelCoord bottomRight(position.x + pixelSize, position.y + pixelSize);
    renderer.drawRect(bottomRight, PixelCoord(12, 4), PixelCoord(12, 4), 0.f, color);
    renderer.drawRect(bottomRight, PixelCoord(4, 12), PixelCoord(4, 12), 0.f, color);
}

void Schematic::drawRange(Renderer& renderer, const PixelCoord center, const float range) {
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

void Schematic::drawBlock(const Presets& presets, Renderer& renderer, const TileCoord tile,
    const BlockPresetID presetID, const BlockRot rotation, const uint32_t color, const bool showRange) {
    const BlockPreset& preset = presets.getBlock(presetID);
    const PixelCoord size = t1::TILE_PC * static_cast<float>(preset.size);
    const PixelCoord position = t1::pixel(tile);
    //
    const float angleRad = preset.rotatable ? static_cast<float>(rotation) * t1::TAU : 0.f;
    const float baseAngleRad = preset.archetype != BlockType::turret ? angleRad : 0.f;
    const PixelCoord origin = size / 2.f;
    renderer.draw(preset.visual.textureRect, position + origin, size, origin, baseAngleRad, color);
    if (preset.archetype == BlockType::turret) {
        const TurretPreset& turret = presets.getTurret(preset.turret);
        const PixelCoord blockCenter = position + origin;
        renderer.draw(turret.visual.textureRect, blockCenter, turret.visual.size, turret.visual.origin, angleRad, color);
        if (showRange)
            drawRange(renderer, t1::tileCenter(tile), turret.range);
    }
}

void Schematic::drawGhosts(Renderer& renderer, const Presets& presets, const uint64_t timeMs) const {
    const float modifier = std::sin(static_cast<float>(timeMs) / 500.f) * 64.f;
    const uint8_t alpha = uint8_t(modifier) + 191; // 255 - 64
    const uint32_t color = 0xFF'FF'FF'00 + alpha;
    for (const auto& blueprint : blueprints) {
        if (blueprint.action == BPAction::build)
            drawBlock(presets, renderer, blueprint.tile, blueprint.presetID, blueprint.rotation, color, false);
        else
            drawBlockFrame(renderer, blueprint.tile, blueprint.size, 0x84'34'34'C8);
    }
}

void Schematic::removeByArea(const TileCoord start, const TileCoord end, const BPAction action) noexcept {
    for (size_t i = 0; i < blueprints.size(); ) {
        const auto& bp = blueprints[i];
        const int bpMaxX = bp.tile.x + bp.size - 1;
        const int bpMaxY = bp.tile.y + bp.size - 1;
        const bool intersects = (bp.tile.x <= end.x && bpMaxX >= start.x) &&
            (bp.tile.y <= end.y && bpMaxY >= start.y);
        if (!intersects || bp.action != action)
            ++i;
        else {
            std::swap(blueprints[i], blueprints.back());
            blueprints.pop_back();
        }
    }
}

void Schematic::drawCancelArea(Renderer& renderer, const TileCoord start, const TileCoord end) {
    for (const auto& bp : blueprints) {
        const int bpMaxX = bp.tile.x + bp.size - 1;
        const int bpMaxY = bp.tile.y + bp.size - 1;
        const bool intersects = (bp.tile.x <= end.x && bpMaxX >= start.x) &&
            (bp.tile.y <= end.y && bpMaxY >= start.y);
        if (intersects)
            drawBlockFrame(renderer, bp.tile, bp.size, 0x84'34'34'C8);
    }
}
