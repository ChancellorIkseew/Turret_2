#pragma once
#include <cstdint>
#include <vector>
#include "engine/assets/preset_defs.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "engine/coords/tile_coord.hpp"

class Assets;
class BlockMap;
class Camera;
class Renderer;

class BlocksDrawer {
    std::vector<TileCoord> cashedTiles;
    //
    std::vector<PixelCoord> itemPositions;
    std::vector<ItemPresetID> itemPresetIDs;
public:
    void draw(const BlockMap& blocks, const Assets& assets, const Camera& camera, Renderer& renderer);
    void addItem(ItemPresetID itemPresetID, PixelCoord position) {
        itemPresetIDs.push_back(itemPresetID);
        itemPositions.push_back(position);
    }
};
