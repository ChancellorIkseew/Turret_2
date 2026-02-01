#pragma once
#include <map>
#include <vector>
#include "engine/coords/tile_coord.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "engine/render/texture.hpp"

class Assets;
class Camera;
class Renderer;
class WorldMap;

class MapDrawer {
    std::map<uint8_t, std::vector<PixelCoord>> cachedFloor;
    std::map<uint8_t, std::vector<PixelCoord>> cachedOverlay;
    TileCoord cashedStart, cashedEnd;
    std::map<uint8_t, Texture> floorTextures;
    std::map<uint8_t, Texture> overlayTextures;
    std::vector<float> positions;
    std::vector<float> uvs;
    std::vector<int> indexCache;
    PixelCoord atlasSize;
public:
    MapDrawer(const Assets& atlas);
    void cacheFloor(const WorldMap& map);
    void cacheOverlay(const WorldMap& map);
    void draw(const Camera& camera, const Renderer& renderer, const WorldMap& map);
private:
    void renderLayer(
        const Renderer& renderer,
        const std::map<uint8_t, std::vector<PixelCoord>>& cachedLayer,
        const std::map<uint8_t, Texture>& textures,
        const PixelCoord tileSize,
        const PixelCoord translation);
};
