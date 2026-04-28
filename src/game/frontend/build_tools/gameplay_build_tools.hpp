#pragma once
#include "build_tools.hpp"

class GameplayBuildTools : public BuildTools {
    //std::optional<TileData> optTileData;
    //BlockRot rotation = BlockRot::up;
    std::optional<TileCoord> optBuildStart;
    std::optional<TileCoord> optDemolishStart;
    std::vector<TileCoord> blueprint;
    TileCoord targetTile;
public:
    GameplayBuildTools() : BuildTools(JEIContent::only_blocks) {}
    //
    virtual void update(Engine& engine) final;
    virtual void drawBlueprint(Engine& engine, Renderer& renderer) final;
private:
    void usePipette(const BlockMap& blocks, const TileCoord tile);
    void demolish(WorldMap& map, BlockMap& blocks, const TileCoord start, const TileCoord end) const;
    void buildBlueprint(GameSession& session, const TileData tileData) const;
    //
    void updateBlueprint(const TileCoord start, TileCoord target);
};
