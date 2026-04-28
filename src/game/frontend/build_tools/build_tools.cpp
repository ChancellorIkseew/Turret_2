#include "build_tools.hpp"

void BuildTools::build(GameSession& session, const TileCoord tile, const TileData tileData) const {
    WorldMap& map = session.getWorld().getMap();
    switch (tileData.component) {
    case TileComponent::floor:   map.placeFloor(tile, tileData.id);                break;
    case TileComponent::overlay: map.placeOverlay(tile, OrePresetID(tileData.id)); break;
    case TileComponent::block: {
        const TeamID teamID = session.getPlayerController().getPlayerTeam()->getID();
        session.getBuiltInScripts().placeBlock(BlockPresetID(tileData.id), tile, teamID, rotation);
        break;
    }
    }
}

void BuildTools::drawOneBlock(Engine& engine, Renderer& renderer, const TileCoord tile) const {
    const BlockPreset& preset = engine.getAssets().getPresets().getBlock(BlockPresetID(optTileData.value().id));
    const PixelCoord size = preset.visual.size;
    const PixelCoord position = t1::pixel(tile);

    const bool canBuild = engine.getSession().getWorld().getBlocks().isAir(tile);

    if (canBuild) renderer.setColorModifier(127, 127, 127, 127);
    else          renderer.setColorModifier(180, 52, 52, 200);

    if (!preset.rotatable)
        renderer.drawFast(preset.visual.texture, position, size);
    else {
        const float angleRad = static_cast<float>(rotation) * t1::PI_F / 2.f;
        const PixelCoord origin = size / 2.f;
        renderer.draw(preset.visual.texture, position + origin, size, origin, angleRad);
    }
    renderer.resetColorModifier();
}
