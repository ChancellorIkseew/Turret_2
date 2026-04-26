#pragma once
#include "engine/engine.hpp"
#include "engine/game_session.hpp"

enum class JEIContent : uint8_t { all, only_blocks };
enum class TileComponent : uint8_t { floor, overlay, block };

struct TileData {
    TileComponent component = TileComponent::floor;
    uint8_t id = 0U;
    constexpr bool operator==(const TileData& other) const noexcept = default;
};

class BuildTools {
    std::optional<TileData> optTileData;
    BlockRot rotation = BlockRot::up;
    JEIContent content;
    std::optional<TileCoord> optBuildStart;
    std::vector<TileCoord> blueprint;
public:
    BuildTools(JEIContent content) : content(content) {}
    //
    JEIContent getContentLevel() const noexcept { return content; }
    void setTileData(const TileData tileData) {
        if (optTileData && optTileData.value() == tileData)
            return optTileData.reset();
        optTileData = tileData;
    }
    //
    void update(Engine& engine);
    void drawBlueprint(Engine& engine, const Renderer& renderer);
private:
    void usePipette(const BlockMap& blocks, const TileCoord tile);
    void build(GameSession& session, const TileCoord tile, const TileData tileData) const;
    void demolish(WorldMap& map, BlockMap& blocks, const TileCoord tile) const;
    //
    void updateBlueprint(const TileCoord start, TileCoord target);
    void drawOneBlock(Engine& engine, const Renderer& renderer, const TileCoord tile) const;
};
