#include "build_tools.hpp"

static TileCoord calculateStep(const TileCoord start, const TileCoord target) {
    const TileCoord delta = target - start;
    if (delta.x != 0 && std::abs(delta.x) > std::abs(delta.y))
        return TileCoord(delta.x / std::abs(delta.x), 0);
    if (delta.y != 0)
        return TileCoord(0, delta.y / std::abs(delta.y));
    return TileCoord(0, 0);
}

void BuildTools::updateBlueprint(const TileCoord start, TileCoord target) {
    const bool hor = std::abs(target.x - start.x) > std::abs(target.y - start.y);
    if (hor) target.y = start.y;
    else     target.x = start.x;
    //
    const TileCoord step = calculateStep(start, target);
    const int len = std::abs(hor ? target.x - start.x : target.y - start.y);
    //
    blueprint.clear();
    for (int i = 0; i <= len; ++i) {
        blueprint.push_back(start + step * i);
    }
}

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
    if (input.active(Demolish))
        demolish(map, blocks, tile);
    // TODO: update and refactoring for building functions 
    if (optTileData && input.jactive(Build))
        optBuildStart = tile;
    if (optTileData && optBuildStart && input.active(Build))
        updateBlueprint(optBuildStart.value(), tile);
    if (optTileData && optBuildStart && input.released(Build)) {
        buildBlueprint(session, optTileData.value());
        optBuildStart.reset();
        blueprint.clear();
    }
}

void BuildTools::usePipette(const BlockMap& blocks, const TileCoord tile) {
    if (blocks.isAir(tile)) {
        optTileData.reset();
        optBuildStart.reset();
        blueprint.clear();
    } 
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

void BuildTools::buildBlueprint(GameSession& session, const TileData tileData) const {
    for (const TileCoord tile : blueprint) {
        build(session, tile, tileData);
    }
}

void BuildTools::drawBlueprint(Engine& engine, const Renderer& renderer) {
    if (!optTileData || !optBuildStart)
        return;
    for (const TileCoord tile : blueprint) {
        drawOneBlock(engine, renderer, tile);
    }
}

void BuildTools::drawOneBlock(Engine& engine, const Renderer& renderer, const TileCoord tile) const {
    const BlockPreset& preset = engine.getAssets().getPresets().getBlock(BlockPresetID(optTileData.value().id));
    const Camera& camera = engine.getSession().getCamera();
    const PixelCoord size = preset.visual.size * camera.getMapScale();
    const PixelCoord position = camera.fromMapToScreen(t1::pixel(tile));

    const bool canBuild = engine.getSession().getWorld().getBlocks().isAir(tile);

    if (canBuild) const_cast<Renderer&>(renderer).setColorModifier(127, 127, 127, 127);
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
