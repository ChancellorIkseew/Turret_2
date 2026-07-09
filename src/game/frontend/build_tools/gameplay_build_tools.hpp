#pragma once
#include "build_tools.hpp"

class Blueprints;

class GameplayBuildTools : public BuildTools {
    std::optional<TileCoord> optBuildStart;
    std::optional<TileCoord> optDemolishStart;
    std::vector<TileCoord> draft;
    TileCoord targetTile;
public:
    GameplayBuildTools() : BuildTools(JEIContent::only_blocks) {}
    //
    virtual void update(Engine& engine) final;
    virtual void drawDraft(Engine& engine, Renderer& renderer) final;
private:
    void usePipette(const BlockMap& blocks, Blueprints& blueprints, const TileCoord tile);
    void demolish(WorldMap& map, BlockMap& blocks, Blueprints& blueprints, const TileCoord start, const TileCoord end) const;
    void buildDraft(World& world, const TileData tileData) const;
    //
    void updateDraft(const TileCoord start, TileCoord target);
};
