#pragma once
#include <cstdint>
#include <vector>
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
    std::vector<uint8_t> itemIDs;
public:
    void draw(const BlockMap& blocks, const Assets& assets, const Camera& camera, Renderer& renderer);
    void addItem(uint8_t itemID, PixelCoord position) {
        itemIDs.push_back(itemID);
        itemPositions.push_back(position);
    }
};
