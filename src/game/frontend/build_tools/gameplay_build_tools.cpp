#include "gameplay_build_tools.hpp"

using GBuildTools = GameplayBuildTools;

static TileCoord calculateStep(const TileCoord start, const TileCoord target) {
    const TileCoord delta = target - start;
    if (delta.x != 0 && std::abs(delta.x) > std::abs(delta.y))
        return TileCoord(delta.x / std::abs(delta.x), 0);
    if (delta.y != 0)
        return TileCoord(0, delta.y / std::abs(delta.y));
    return TileCoord(0, 0);
}

void GBuildTools::updateDraft(const TileCoord start, TileCoord target) {
    const bool hor = std::abs(target.x - start.x) > std::abs(target.y - start.y);
    if (hor) target.y = start.y;
    else     target.x = start.x;
    //
    const TileCoord step = calculateStep(start, target);
    const int len = std::abs(hor ? target.x - start.x : target.y - start.y);
    //
    draft.clear();
    for (int i = 0; i <= len; ++i) {
        draft.push_back(start + step * i);
    }
}

void GBuildTools::update(Engine& engine) {
    const Input& input = engine.getMainWindow().getInput();
    GameSession& session = engine.getSession();
    WorldMap& map = session.getWorld().getMap();
    BlockMap& blocks = session.getWorld().getBlocks();
    Blueprints& blueprints = session.getWorld().getBlueprints();
    targetTile = t1::tile(session.getCamera().fromScreenToMap(input.getMouseCoord()));
    //
    if (input.jactive(Rotate_building))
        rotation = static_cast<BlockRot>((rotation + 1) % 4);
    if (input.jactive(Pipette))
        usePipette(blocks, targetTile);
    // Build:
    if (optTileData && input.jactive(Build))
        optBuildStart = targetTile;
    if (optTileData && optBuildStart && input.active(Build))
        updateDraft(optBuildStart.value(), targetTile);
    if (optTileData && optBuildStart && input.released(Build)) {
        buildDraft(session, optTileData.value());
        optBuildStart.reset();
        draft.clear();
    }
    // Demolish:
    if (input.jactive(Demolish))
        optDemolishStart = targetTile;
    if (optDemolishStart && input.released(Demolish)) {
        demolish(map, blocks, blueprints, optDemolishStart.value(), targetTile);
        optDemolishStart.reset();
    }
}

void GBuildTools::usePipette(const BlockMap& blocks, const TileCoord tile) {
    if (blocks.isAir(tile)) {
        optTileData.reset();
        optBuildStart.reset();
        draft.clear();
    }
    else if (blocks.contains(tile)) {
        const auto& block = blocks.at(tile).block;
        optTileData = TileData(TileComponent::block, block->presetID.asUint());
        if (block->getRotation() != BlockRot::none)
            rotation = block->getRotation();
    }
}

void GBuildTools::demolish(WorldMap& map, BlockMap& blocks, Blueprints& blueprints, const TileCoord start, const TileCoord end) const {
    const TileCoord nStart = TileCoord(std::min(start.x, end.x), std::min(start.y, end.y));
    const TileCoord nEnd = TileCoord(std::max(start.x, end.x), std::max(start.y, end.y));
    for (int x = nStart.x; x <= nEnd.x; ++x) {
        for (int y = nStart.y; y <= nEnd.y; ++y) {
            const TileCoord tile(x, y);
            if (blocks.isFilled(tile))
                blocks.demolish(tile);
            blueprints.removeIfExists(tile);
        }
    }
}

void GBuildTools::buildDraft(GameSession& session, const TileData tileData) const {
    for (const TileCoord tile : draft) {
        if (session.getWorld().getBlocks().isAir(tile))
            session.getWorld().getBlueprints().addOrReplace(tile, BlockPresetID(tileData.id), rotation);
    }
}

static void drawDemolitonRect(const Renderer& renderer, const TileCoord start, const TileCoord end) {
    const TileCoord nStart = TileCoord(std::min(start.x, end.x), std::min(start.y, end.y));
    const TileCoord size = TileCoord(std::abs(start.x - end.x) + 1, std::abs(start.y - end.y) + 1);
    renderer.drawRect(0x84'34'34'C8, t1::pixel(nStart), t1::pixel(size)); // maybe add hex->rgba and rgba->hex
}

void GBuildTools::drawDraft(Engine& engine, Renderer& renderer) {
    const Camera& camera = engine.getSession().getCamera();
    renderer.setTranslation(camera.getTranslation());
    renderer.setScale(camera.getMapScale());
    if (optDemolishStart)
        drawDemolitonRect(renderer, optDemolishStart.value(), targetTile);
    if (optTileData) {
        if (!optBuildStart)
            drawOneBlock(engine, renderer, targetTile, optTileData.value());
        else {
            for (const TileCoord tile : draft) {
                drawOneBlock(engine, renderer, tile, optTileData.value());
            }
        }
    }
    renderer.setTranslation(PixelCoord(0, 0));
    renderer.setScale(1.f);
}
