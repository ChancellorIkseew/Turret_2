#pragma once
#include "build_tools.hpp"

class EditorBuildTools : public BuildTools {
    TileCoord targetTile;
public:
    EditorBuildTools() : BuildTools(JEIContent::all) {}

    virtual void update(Engine& engine) final {
        const Input& input = engine.getMainWindow().getInput();
        GameSession& session = engine.getSession();
        WorldMap& map = session.getWorld().getMap();
        BlockMap& blocks = session.getWorld().getBlocks();
        targetTile = t1::tile(session.getCamera().fromScreenToMap(input.getMouseCoord()));
        //
        if (input.jactive(Rotate_building))
            rotation = static_cast<BlockRot>((rotation + 1) % 4);
        if (input.jactive(Pipette))
            usePipette(map, blocks, targetTile);
        if (optTileData && input.active(Build))
            build(session, targetTile, optTileData.value());
        if (input.active(Demolish))
            demolish(map, blocks, targetTile);
    }

    virtual void drawBlueprint(Engine& engine, Renderer& renderer) final {
        const Camera& camera = engine.getSession().getCamera();
        renderer.setTranslation(camera.getTranslation());
        renderer.setScale(camera.getMapScale());
        if (optTileData)
            drawOneBlock(engine, renderer, targetTile, optTileData.value());
        renderer.setTranslation(PixelCoord(0, 0));
        renderer.setScale(1.f);
    }

    void demolish(WorldMap& map, BlockMap& blocks, const TileCoord tile) {
        if (blocks.isFilled(tile))
            return blocks.demolish(tile);
        if (map.tileExists(tile))
            return map.placeOverlay(tile, OrePresetID(0));
    }

    void usePipette(WorldMap& map, BlockMap& blocks, const TileCoord tile) {
        if (blocks.isFilled(tile)) {
            const auto& block = blocks.at(tile).block;
            optTileData = TileData(TileComponent::block, block->presetID.asUint());
            if (block->getRotation() != BlockRot::none)
                rotation = block->getRotation();
            return;
        }
        if (map.tileExists(tile)) {
            if (map.at(tile).ore != OrePresetID(0))
                optTileData = TileData(TileComponent::overlay, map.at(tile).ore.asUint());
            else
                optTileData = TileData(TileComponent::floor, map.at(tile).floor);
        }
    }
};
