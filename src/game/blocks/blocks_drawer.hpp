#pragma once
#include <vector>
#include "engine/coords/tile_coord.hpp"

class Assets;
class BlockMap;
class Camera;
class Renderer;

class BlockDrawer {
    std::vector<TileCoord> cashedTiles;
public:
    void draw(const BlockMap& blocks, const Assets& assets, const Camera& camera, Renderer& renderer);
};
