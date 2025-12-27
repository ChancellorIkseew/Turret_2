#pragma once
#include "config.hpp"
#include "tile_coord.hpp"
#include "pixel_coord.hpp"

namespace t1 {
    constexpr int TILE = 32;
    constexpr int HALF_TILE = 16;

    // pixel_to_tile
    t1_finline int tile(const float pixelCoord) noexcept {
        return static_cast<int>(pixelCoord) / TILE;
    }
    t1_finline int tile(const int pixelCoord) noexcept {
        return pixelCoord / TILE;
    }
    t1_finline TileCoord tile(const float pixelCoordX, const float pixelCoordY) noexcept {
        return TileCoord(tile(pixelCoordX), tile(pixelCoordY));
    }
    t1_finline TileCoord tile(const int pixelCoordX, const int pixelCoordY) noexcept {
        return TileCoord(tile(pixelCoordX), tile(pixelCoordY));
    }
    t1_finline TileCoord tile(const PixelCoord pixelCoord) noexcept {
        return TileCoord(tile(pixelCoord.x), tile(pixelCoord.y));
    }

    // tile_to_pixel
    t1_finline int pixel(const int tileCoord) noexcept {
        return tileCoord * TILE;
    }
    t1_finline float pixelF(const int tileCoord) noexcept {
        return static_cast<float>(tileCoord * TILE);
    }
    t1_finline PixelCoord pixel(const int tileCoordX, const int tileCoordY) noexcept {
        return PixelCoord(pixelF(tileCoordX), pixelF(tileCoordY));
    }
    t1_finline PixelCoord pixel(const TileCoord tileCoord) noexcept {
        return PixelCoord(pixelF(tileCoord.x), pixelF(tileCoord.y));
    }

    // tile_centre
    t1_finline int tileCentre(const int tileCoord) noexcept {
        return tileCoord * TILE + HALF_TILE;
    }
    t1_finline float tileCentreF(const int tileCoord) noexcept {
        return static_cast<float>(tileCoord * TILE + HALF_TILE);
    }
    t1_finline PixelCoord tileCentre(const int tileCoordX, const int tileCoordY) noexcept {
        return PixelCoord(tileCentreF(tileCoordX), tileCentreF(tileCoordY));
    }
    t1_finline PixelCoord tileCentre(const TileCoord tileCoord) noexcept {
        return PixelCoord(tileCentreF(tileCoord.x), tileCentreF(tileCoord.y));
    }

    // simple math
    t1_finline bool areCloser(const PixelCoord first, const PixelCoord second, const float distance) noexcept {
        return abs(first.x - second.x) < distance && abs(first.y - second.y) < distance;
    }

    t1_finline bool contains(const TileCoord start, const TileCoord end, const TileCoord target) noexcept {
        return target.x >= start.x && target.x <= end.x &&
               target.y >= start.y && target.y <= end.y;
    }
    t1_finline bool contains(const PixelCoord start, const PixelCoord end, const PixelCoord target) noexcept {
        return target.x >= start.x && target.x <= end.x &&
               target.y >= start.y && target.y <= end.y;
    }
};
