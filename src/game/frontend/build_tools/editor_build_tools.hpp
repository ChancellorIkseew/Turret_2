#pragma once
#include "build_tools.hpp"

class EditorBuildTools : public BuildTools {
    TileCoord targetTile;
    bool lastDeletedWasBlock = false;
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
        if (optTileData && input.active(Build_Shoot))
            build(session, targetTile, optTileData.value());
        // fixes block and ore demolishing in one click, maybe needs refactoring
        const bool currentIsBlock = blocks.isFilled(targetTile);
        const bool sameComponent = currentIsBlock == lastDeletedWasBlock;
        if (input.jactive(Demolish) || input.active(Demolish) && sameComponent) {
            lastDeletedWasBlock = currentIsBlock;
            demolish(map, blocks, targetTile);
        }
    }

    virtual void drawDraft(Engine& engine, Renderer& renderer) final {
        if (optTileData)
            drawOneBlock(engine, renderer, targetTile, optTileData.value());
    }
private:
    void build(GameSession& session, const TileCoord tile, const TileData tileData) const {
        WorldMap& map = session.getWorld().getMap();
        switch (tileData.component) {
        case TileComponent::floor:   map.placeFloor(tile, tileData.id);                break;
        case TileComponent::overlay: map.placeOverlay(tile, OrePresetID(tileData.id)); break;
        case TileComponent::block: {
                const TeamID teamID = session.getPlayerController().getPlayerTeamID();
                session.getBuiltInScripts().placeBlock(BlockPresetID(tileData.id), tile, teamID, rotation);
            }
            break;
        }
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
