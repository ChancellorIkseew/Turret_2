#pragma once
#include <map>
#include "engine/render/sprite.hpp"
#include "world.hpp"

class Camera;

class MapDrawer {
    using WorldMap = std::vector<std::vector<MapTile>>;
    //
    const Camera& camera;
    const WorldMap& map;
    std::map<TileType, std::vector<PixelCoord>> layers;
    TileCoord cashedStart, cashedEnd;
    Sprite sprite;
public:
    MapDrawer(const Camera& camera, const WorldMap& map);
    void cacheLayers();
    void draw();
    void drawStructures();
};
