#include "gameplay_build_tools.hpp"
//
#include "engine/gui/gui.hpp"

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
    const bool mouseFree = !engine.getGUI().ownsMouse();
    GameSession& session = engine.getSession();
    WorldMap& map = session.getWorld().getMap();
    BlockMap& blocks = session.getWorld().getBlocks();
    Blueprints& blueprints = session.getWorld().getBlueprints();
    session.getPlayerController().setHoldsBlock(optTileData.operator bool());
    targetTile = t1::tile(session.getCamera().fromScreenToMap(input.getMouseCoord()));
    //
    if (input.jactive(Rotate_building))
        rotation = static_cast<BlockRot>((rotation + 1) % 4);
    if (input.jactive(Pipette) && mouseFree)
        usePipette(blocks, blueprints, targetTile);
    // Build:
    if (optTileData && input.jactive(Build_Shoot) && mouseFree)
        optBuildStart = targetTile;
    if (optTileData && optBuildStart && input.active(Build_Shoot))
        updateDraft(optBuildStart.value(), targetTile);
    if (optTileData && optBuildStart && input.released(Build_Shoot)) {
        buildDraft(session, optTileData.value());
        optBuildStart.reset();
        draft.clear();
    }
    // Demolish:
    if (input.jactive(Demolish) && mouseFree)
        optDemolishStart = targetTile;
    if (optDemolishStart && input.released(Demolish)) {
        demolish(map, blocks, blueprints, optDemolishStart.value(), targetTile);
        optDemolishStart.reset();
    }

    if (!engine.getGUI().ownsMouse()) {

    }

}

void GBuildTools::usePipette(const BlockMap& blocks, Blueprints& blueprints, const TileCoord tile) {
    if (blocks.isAir(tile) && blueprints.isAir(tile)) {
        optTileData.reset();
        optBuildStart.reset();
        draft.clear();
        return;
    }

    if (!blocks.isAir(tile)) {
        const auto& block = blocks.at(tile).block;
        optTileData = TileData(TileComponent::block, block->presetID.asUint());
        if (block->getRotation() != BlockRot::none)
            rotation = block->getRotation();
    } else if (!blueprints.isAir(tile)) {
        const auto& block = blueprints.getBlock(tile);
        optTileData = TileData(TileComponent::block, block.presetID.asUint());
        if (block.rotation != BlockRot::none)
            rotation = block.rotation;
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

static void drawDemolitonRect(Renderer& renderer, const TileCoord start, const TileCoord end) {
    const TileCoord nStart = TileCoord(std::min(start.x, end.x), std::min(start.y, end.y));
    const TileCoord size = TileCoord(std::abs(start.x - end.x) + 1, std::abs(start.y - end.y) + 1);
    constexpr PixelCoord ORIGIN(0.f, 0.f);
    renderer.drawRect(t1::pixel(nStart), t1::pixel(size), ORIGIN, 0.f, 0x84'34'34'C8);
}

void GBuildTools::drawDraft(Engine& engine, Renderer& renderer) {
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
}
