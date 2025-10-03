#include "world_map.hpp"
//
#include <iostream>

WorldMap::WorldMap(const TileCoord mapSize) : terrain(mapSize.x * mapSize.y), mapSize(mapSize) {
    content::load(registry);
    //WorldMap::at(10, 10).block = new Block("round_shadow", "ice_block", TileCoord(10, 10));
}

void WorldMap::print() {
    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            std::cout << static_cast<int>(WorldMap::at(x, y).floor);
        }
        std::cout << '\n';
    }
}

void WorldMap::placeFloor(const TileCoord tile, const uint8_t floorID) {
    if (tileExists(tile))
        WorldMap::at(tile).floor = floorID;
}
void WorldMap::placeOverlay(const TileCoord tile, const uint8_t overlayID) {
    if (tileExists(tile))
        WorldMap::at(tile).overlay = overlayID;
}
