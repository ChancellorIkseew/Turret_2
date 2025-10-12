#include "world_map.hpp"
//
#include <iostream>
#include "game/events/events.hpp"

WorldMap::WorldMap(const TileCoord mapSize) : terrain(mapSize.x * mapSize.y), mapSize(mapSize) {
    content::load(indexes);
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
    if (!tileExists(tile))
        return;
    WorldMap::at(tile).floor = floorID;
    Events::setActive(Event::floor_changed);
    Events::setActive(Event::map_changed);
}
void WorldMap::placeOverlay(const TileCoord tile, const uint8_t overlayID) {
    if (!tileExists(tile))
        return;
    WorldMap::at(tile).overlay = overlayID;
    Events::setActive(Event::overlay_changed);
    Events::setActive(Event::map_changed);
}
