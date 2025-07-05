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
    const ElementRegistry& reg;
    std::map<uint8_t, std::vector<PixelCoord>> layers;
    std::map<uint8_t, std::vector<PixelCoord>> ores;
    TileCoord cashedStart, cashedEnd;
    Sprite sprite;
public:
    MapDrawer(const Camera& camera, const World& world);
    void cacheLayers();
    void cacheOres();
    void draw();
    //void drawOres();
    void drawStructures();
};
