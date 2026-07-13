#pragma once
#include <cstdint>
#include <vector>
#include "engine/assets/preset_defs.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "engine/coords/tile_coord.hpp"

class BlockMap;
class Camera;
class Presets;
class Renderer;

class BlocksDrawer {
    std::vector<TileCoord> cashedTiles;
    //
    std::vector<TileCoord> inProgress;
    std::vector<PixelCoord> itemPositions;
    std::vector<ItemPresetID> itemPresetIDs;
public:
    void drawShadows(const BlockMap& blocks, const Camera& camera, Renderer& renderer);
    void draw(const BlockMap& blocks, const Presets& presets, Renderer& renderer);
    void drawInProgress(const BlockMap& blocks, const Presets& presets, Renderer& renderer);
    void addInProgress(const TileCoord tile) {
        inProgress.push_back(tile);
    }
    void addItem(ItemPresetID itemPresetID, PixelCoord position) {
        itemPresetIDs.push_back(itemPresetID);
        itemPositions.push_back(position);
    }
};
