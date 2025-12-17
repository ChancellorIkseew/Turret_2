#pragma once
#include <map>
#include <vector>
#include "engine/coords/tile_coord.hpp"
#include "engine/render/sprite.hpp"

struct MapTile;
class Camera;
class WorldMap;

class MapDrawer {
    std::map<uint8_t, std::vector<PixelCoord>> cachedFloor;
    std::map<uint8_t, std::vector<PixelCoord>> cachedOverlay;
    TileCoord cashedStart, cashedEnd;
    Sprite sprite;
    std::map<uint8_t, Texture> floorTextures;
    std::map<uint8_t, Texture> overlayTextures;
public:
    MapDrawer();
    void cacheFloor(const WorldMap& map);
    void cacheOverlay(const WorldMap& map);
    void draw(const Camera& camera, const WorldMap& map);
    void drawStructures(const Camera& camera, const WorldMap& map);
};
