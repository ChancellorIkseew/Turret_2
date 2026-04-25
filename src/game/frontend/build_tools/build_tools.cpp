#include "build_tools.hpp"

void BuildTools::update(Engine& engine) {
    const Input& input = engine.getMainWindow().getInput();
    GameSession& session = engine.getSession();
    WorldMap& map = session.getWorld().getMap();
    BlockMap& blocks = session.getWorld().getBlocks();
    const TileCoord tile = t1::tile(session.getCamera().fromScreenToMap(input.getMouseCoord()));
    //
    if (input.jactive(Rotate_building))
        rotation = static_cast<BlockRot>((rotation + 1) % 4);
    if (input.jactive(Pipette))
        usePipette(blocks, tile);
    if (optTileData && input.active(Build))
        build(session, tile, optTileData.value());
    if (input.active(Demolish))
        demolish(map, blocks, tile);
}

void BuildTools::usePipette(const BlockMap& blocks, const TileCoord tile) {
    if (blocks.isAir(tile))
        optTileData.reset();
    else if (blocks.contains(tile)) {
        const auto& block = blocks.at(tile).block;
        optTileData = TileData(TileComponent::block, block->presetID.asUint());
        if (block->getRotation() != BlockRot::none)
            rotation = block->getRotation();
    }
}

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

void BuildTools::demolish(WorldMap& map, BlockMap& blocks, const TileCoord tile) const {
    if (blocks.isFilled(tile))
        return blocks.demolish(tile);
    if (content == JEIContent::all && map.tileExists(tile))
        return map.placeOverlay(tile, OrePresetID(0));
    // TODO: add area demolish
}

void BuildTools::drawBlock(Engine& engine, const Renderer& renderer, const PixelCoord mousePosition) {
    if (!optTileData)
        return;
    const BlockPreset& preset = engine.getAssets().getPresets().getBlock(BlockPresetID(optTileData.value().id));
    const Camera& camera = engine.getSession().getCamera();
    const PixelCoord size = preset.visual.size * camera.getMapScale();

    const TileCoord targetTile = t1::tile(camera.fromScreenToMap(mousePosition));
    const PixelCoord position = camera.fromMapToScreen(t1::pixel(targetTile));

    const bool canBuild = engine.getSession().getWorld().getBlocks().isAir(targetTile);

    if (canBuild) const_cast<Renderer&>(renderer).setColorModifier(255, 255, 255, 255);
    else          const_cast<Renderer&>(renderer).setColorModifier(180, 52, 52, 200);

    if (!preset.rotatable)
        renderer.drawFast(preset.visual.texture, position, size);
    else {
        const float angleRad = static_cast<float>(rotation) * t1::PI_F / 2.f;
        const PixelCoord origin = size / 2.f;
        renderer.draw(preset.visual.texture, position + origin, size, origin, angleRad);
    }
    const_cast<Renderer&>(renderer).resetColorModifier();
}
