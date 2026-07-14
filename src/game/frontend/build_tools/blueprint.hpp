#pragma once
#include <vector>
#include "engine/coords/math.hpp"
#include "game/blocks/block.hpp"

class Presets;
class Renderer;

struct Blueprint {
    TileCoord tile;
    BlockPresetID presetID = BlockPresetID(0);
    BlockRot rotation = BlockRot::up;
    BPAction action = BPAction::build;

    constexpr bool operator==(const TileCoord& otherTile) const noexcept {
        return tile == otherTile;
    }
};

class Blueprints {
    static constexpr size_t MAX_ELEMENTS = 512;
    std::vector<Blueprint> blueprints;
    //
    constexpr auto findByTile(const TileCoord tile) noexcept {
        return std::find(blueprints.begin(), blueprints.end(), tile);
    }
public:
    void addOrReplace(const TileCoord tile, const BlockPresetID presetID, const BlockRot rotation, const BPAction action) {
        auto it = findByTile(tile);
        if (it != blueprints.end()) // replace
             *it = Blueprint(tile, presetID, rotation, action);
        else if (blueprints.size() < MAX_ELEMENTS) // emplace
            blueprints.emplace_back(tile, presetID, rotation, action);
    }

    void removeIfExists(const TileCoord tile) noexcept {
        auto it = findByTile(tile);
        if (it == blueprints.end())
            return;
        if (it != blueprints.end() - 1)
            *it = std::move(blueprints.back());
        blueprints.pop_back();
    }
    
    Blueprint* getClosest(const TileCoord target) noexcept {
        int minSqrDistance = std::numeric_limits<int>::max();
        Blueprint* closest = nullptr;
        for (auto& bp : blueprints) {
            const int sqrDistance = t1::pow2i(bp.tile.x - target.x) + t1::pow2i(bp.tile.y - target.y);
            if (sqrDistance < minSqrDistance) {
                minSqrDistance = sqrDistance;
                closest = &bp;
            }
        }
        return closest;
    }

    Blueprint getBlock(const TileCoord tile) {
        auto it = findByTile(tile);
        if (it != blueprints.end())
            return *it;
        return Blueprint(/*null object*/);
    }

    bool isAir(const TileCoord tile) {
        return getBlock(tile).presetID == BlockPresetID(0); // air
    }

    void drawGhosts(Renderer& renderer, const Presets& presets, const uint64_t timeMs) const;
    bool empty() const noexcept { return blueprints.empty(); }

    static void drawRange(Renderer& renderer, const PixelCoord center, const float range);
    static void drawBlock(const Presets& presets, Renderer& renderer, const TileCoord tile,
        const BlockPresetID presetID, const BlockRot rotation, const uint32_t color, const bool showRange);
};
