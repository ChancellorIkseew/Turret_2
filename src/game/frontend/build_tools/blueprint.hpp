#pragma once
#include <vector>
#include "engine/coords/math.hpp"
#include "engine/coords/transforms.hpp"
#include "game/blocks/block.hpp"

class Presets;
class Renderer;

struct Blueprint {
    TileCoord tile;
    PixelCoord center;
    uint8_t size = 1;
    BlockPresetID presetID = BlockPresetID(0);
    BlockRot rotation = BlockRot::up;
    BPAction action = BPAction::build;

    Blueprint() = default;
    Blueprint(const TileCoord tile, const uint8_t size, const BlockPresetID presetID,
        const BlockRot rotation, const BPAction action) :
        tile(tile), size(size), presetID(presetID), rotation(rotation), action(action),
        center(t1::pixel(tile) + t1::pixel(size, size) / 2.f) { }

    constexpr bool operator==(const TileCoord& otherTile) const noexcept {
        return tile == otherTile;
    }

    static constexpr bool intersects(const Blueprint& block, const TileCoord tile, const int size) noexcept {
        return tile.x < block.tile.x + block.size && tile.x + size > block.tile.x &&
            tile.y < block.tile.y + block.size && tile.y + size > block.tile.y;
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
    void addOrReplace(const TileCoord tile, const uint8_t size, const BlockPresetID presetID,
        const BlockRot rotation, const BPAction action) {
        auto it = findByTile(tile);
        if (it != blueprints.end()) // replace
             *it = Blueprint(tile, size, presetID, rotation, action);
        else if (blueprints.size() < MAX_ELEMENTS) // emplace
            blueprints.emplace_back(tile, size, presetID, rotation, action);
    }

    void removeIfExists(const TileCoord tile) noexcept {
        auto it = findByTile(tile);
        if (it == blueprints.end())
            return;
        if (it != blueprints.end() - 1)
            *it = std::move(blueprints.back());
        blueprints.pop_back();
    }
    
    Blueprint* getClosest(const PixelCoord target) noexcept {
        float minSqrDistance = std::numeric_limits<float>::max();
        Blueprint* closest = nullptr;
        for (auto& bp : blueprints) {
            const float sqrDistance = t1::squareDistance(target, bp.center);
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
        return canPlace(tile, 1); // air
    }

    bool canPlace(const TileCoord tile, const int size) {
        for (const auto& block : blueprints) {
            if (Blueprint::intersects(block, tile, size))
                return false;
        }
        return true;
    }

    void drawGhosts(Renderer& renderer, const Presets& presets, const uint64_t timeMs) const;
    bool empty() const noexcept { return blueprints.empty(); }

    static void drawRange(Renderer& renderer, const PixelCoord center, const float range);
    static void drawBlock(const Presets& presets, Renderer& renderer, const TileCoord tile,
        const BlockPresetID presetID, const BlockRot rotation, const uint32_t color, const bool showRange);
};
