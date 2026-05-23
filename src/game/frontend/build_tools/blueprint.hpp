#pragma once
#include <vector>
#include "engine/coords/math.hpp"
#include "game/blocks/block.hpp"

class Engine;
class Renderer;

struct Blueprint {
    TileCoord tile;
    BlockPresetID presetID;
    BlockRot rotation;
    uint8_t progress;

    bool operator==(const TileCoord& otherTile) const noexcept {
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
    void addOrReplace(const TileCoord tile, const BlockPresetID presetID, const BlockRot rotation) {
        auto it = findByTile(tile);
        if (it != blueprints.end()) // replace
            *it = Blueprint(tile, presetID, rotation);
        else if (blueprints.size() < MAX_ELEMENTS) // emplace
            blueprints.emplace_back(tile, presetID, rotation);
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

    void draw(Renderer& renderer, const Engine& engine) const;
    bool empty() const noexcept { return blueprints.empty(); }
};
