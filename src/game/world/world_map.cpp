#include "world_map.hpp"
//
#include "game/events/events.hpp"

WorldMap::WorldMap(const TileCoord mapSize) :
    terrain(mapSize.x * mapSize.y), mapSize(mapSize) { }

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
