#pragma once
#include "config.hpp"
#include "tile_coord.hpp"
#include "pixel_coord.hpp"

namespace t1 {
    constexpr int TILE = 32;
    constexpr float TILE_F = 32.0f;
    constexpr PixelCoord TILE_PC = PixelCoord(TILE_F, TILE_F);
    constexpr int HALF_TILE = 16;

    // pixel_to_tile
    t1_finline_cxpr int tile(const float pixelCoord) noexcept {
        return static_cast<int>(pixelCoord) / TILE;
    }
    t1_finline_cxpr int tile(const int pixelCoord) noexcept {
        return pixelCoord / TILE;
    }
    t1_finline_cxpr TileCoord tile(const float pixelCoordX, const float pixelCoordY) noexcept {
        return TileCoord(tile(pixelCoordX), tile(pixelCoordY));
    }
    t1_finline_cxpr TileCoord tile(const int pixelCoordX, const int pixelCoordY) noexcept {
        return TileCoord(tile(pixelCoordX), tile(pixelCoordY));
    }
    t1_finline_cxpr TileCoord tile(const PixelCoord pixelCoord) noexcept {
        return TileCoord(tile(pixelCoord.x), tile(pixelCoord.y));
    }

    // tile_to_pixel
    t1_finline_cxpr int pixel(const int tileCoord) noexcept {
        return tileCoord * TILE;
    }
    t1_finline_cxpr float pixelF(const int tileCoord) noexcept {
        return static_cast<float>(tileCoord * TILE);
    }
    t1_finline_cxpr PixelCoord pixel(const int tileCoordX, const int tileCoordY) noexcept {
        return PixelCoord(pixelF(tileCoordX), pixelF(tileCoordY));
    }
    t1_finline_cxpr PixelCoord pixel(const TileCoord tileCoord) noexcept {
        return PixelCoord(pixelF(tileCoord.x), pixelF(tileCoord.y));
    }

    // tile_centre
    t1_finline_cxpr int tileCenter(const int tileCoord) noexcept {
        return tileCoord * TILE + HALF_TILE;
    }
    t1_finline_cxpr float tileCenterF(const int tileCoord) noexcept {
        return static_cast<float>(tileCoord * TILE + HALF_TILE);
    }
    t1_finline_cxpr PixelCoord tileCenter(const int tileCoordX, const int tileCoordY) noexcept {
        return PixelCoord(tileCenterF(tileCoordX), tileCenterF(tileCoordY));
    }
    t1_finline_cxpr PixelCoord tileCenter(const TileCoord tileCoord) noexcept {
        return PixelCoord(tileCenterF(tileCoord.x), tileCenterF(tileCoord.y));
    }
};
