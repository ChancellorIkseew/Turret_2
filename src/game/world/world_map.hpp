#pragma once
#include <cstdint>
#include <vector>
#include "engine/coords/tile_coord.hpp"

struct MapTile {
    uint8_t floor = 0;
    uint8_t overlay = 0;
};

class WorldMap {
    std::vector<uint8_t> floor;
    std::vector<uint8_t> overlay;
    const TileCoord mapSize;
public:
    WorldMap(const TileCoord mapSize, std::vector<uint8_t>& floor, std::vector<uint8_t>& overlay);
    //
    void placeFloor(const TileCoord tile, const uint8_t floorID);
    void placeOverlay(const TileCoord tile, const uint8_t overlayID);
    //
    constexpr bool tileExists(const int tileX, const int tileY) const {
        return tileX >= 0 && tileX < mapSize.x &&
               tileY >= 0 && tileY < mapSize.y;
    }
    constexpr bool tileExists(const TileCoord tile) const {
        return tileExists(tile.x, tile.y);
    }
    //
    constexpr MapTile at(const int x, const int y) const noexcept {
        return MapTile(floor[x + y * mapSize.x], overlay[x + y * mapSize.x]);
    }
    constexpr MapTile at(const TileCoord tile) const noexcept {
        return at(tile.x, tile.y);
    }
    //
    TileCoord   getSize()    const noexcept { return mapSize; }
    const auto& getFloor()   const noexcept { return floor; }
    const auto& getOverlay() const noexcept { return overlay; }
};
