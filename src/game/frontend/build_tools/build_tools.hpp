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
protected:
    std::optional<TileData> optTileData;
    BlockRot rotation = BlockRot::up;
    JEIContent content;
public:
    BuildTools(JEIContent content) : content(content) {}
    virtual ~BuildTools() = default;
    //
    JEIContent getContentLevel() const noexcept { return content; }
    void setTileData(const TileData tileData) {
        if (optTileData && optTileData.value() == tileData)
            return optTileData.reset();
        optTileData = tileData;
    }
    //
    virtual void update(Engine& engine) = 0;
    virtual void drawBlueprint(Engine& engine, const Renderer& renderer) = 0;
protected:
    void build(GameSession& session, const TileCoord tile, const TileData tileData) const;
    void drawOneBlock(Engine& engine, const Renderer& renderer, const TileCoord tile) const;
};
