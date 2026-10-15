#pragma once
#include <cstdint>
#include <map>
#include <vector>
#include "engine/coords/tile_coord.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "engine/render/texture_rect.hpp"

class Camera;
class Presets;
class Renderer;
class WorldMap;

class MapDrawer {
    std::map<uint8_t, std::vector<PixelCoord>> cachedFloor;
    TileCoord cashedStart, cashedEnd;
    std::map<uint8_t, TextureRect> floorTextures;
    std::map<uint8_t, TextureRect> oreTextures;
public:
    MapDrawer(const Presets& presets) { updateTextures(presets); }
    void updateTextures(const Presets& assets);
    void cacheFloor(const WorldMap& map);
    void draw(const Camera& camera, Renderer& renderer, const WorldMap& map);
private:
    void renderFloor(Renderer& renderer);
};
