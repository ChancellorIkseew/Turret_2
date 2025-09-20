#pragma once
#include "tile_coord.hpp"
#include "pixel_coord.hpp"

constexpr int TILE = 32;
constexpr int HALF_TILE = 16;

namespace t1 {
    int tile(const float pixelCoord);
    int tile(const int pixelCoord);
    TileCoord tile(const float pixelCoordX, const float pixelCoordY);
    TileCoord tile(const int pixelCoordX, const int pixelCoordY);
    TileCoord tile(const PixelCoord pixelCoord);

    int pixel(const int tileCoord);
    float pixelF(const int tileCoord);
    PixelCoord pixel(const int tileCoordX, const int tileCoordY);
    PixelCoord pixel(const TileCoord tileCoord);

    int tileCentre(const int tileCoord);
    float tileCentreF(const int tileCoord);
    PixelCoord tileCentre(const int tileCoordX, const int tileCoordY);
    PixelCoord tileCentre(const TileCoord tileCoord);

    bool areCloser(const PixelCoord first, const PixelCoord second, const float distance);
};
