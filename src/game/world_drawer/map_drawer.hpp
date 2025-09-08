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
    std::map<uint8_t, std::vector<PixelCoord>> floor;
    std::map<uint8_t, std::vector<PixelCoord>> overlay;
    TileCoord cashedStart, cashedEnd;
    Sprite sprite;
    std::map<uint8_t, Texture> floorTextures;
    std::map<uint8_t, Texture> overlayTextures;
public:
    MapDrawer(const Camera& camera, const World& world);
    void cacheLayers();
    void cacheOres();
    void draw();
    void drawStructures();
};
