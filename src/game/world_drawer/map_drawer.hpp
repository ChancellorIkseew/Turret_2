#pragma once
#include <map>
#include "engine/coords/tile_coord.hpp"
#include "engine/render/sprite.hpp"

struct MapTile;
class Camera;
class World;
class WorldMap;

class MapDrawer {
    const Camera& camera;
    const WorldMap& map;
    std::map<uint8_t, std::vector<PixelCoord>> cachedFloor;
    std::map<uint8_t, std::vector<PixelCoord>> cachedOverlay;
    TileCoord cashedStart, cashedEnd;
    Sprite sprite;
    std::map<uint8_t, Texture> floorTextures;
    std::map<uint8_t, Texture> overlayTextures;
public:
    MapDrawer(const Camera& camera, const World& world);
    void cacheFloor();
    void cacheOverlay();
    void draw();
    void drawStructures();
};
