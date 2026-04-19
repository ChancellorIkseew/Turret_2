#include "world_map.hpp"
//
#include "game/events/events.hpp"

WorldMap::WorldMap(const TileCoord mapSize, std::vector<uint8_t>& floor, std::vector<uint8_t>& overlay) :
    mapSize(mapSize), floor(std::move(floor)), overlay(std::move(overlay)) { }

static constexpr int getIndex(const int x, const int y, const TileCoord mapSize) noexcept {
    return x + y * mapSize.x;
}

void WorldMap::placeFloor(const TileCoord tile, const uint8_t floorID) {
    if (!tileExists(tile))
        return;
    floor[getIndex(tile.x, tile.y, mapSize)] = floorID;
    Events::setActive(Event::floor_changed);
    Events::setActive(Event::map_changed);
}
void WorldMap::placeOverlay(const TileCoord tile, const uint8_t overlayID) {
    if (!tileExists(tile))
        return;
    overlay[getIndex(tile.x, tile.y, mapSize)] = overlayID;
    Events::setActive(Event::overlay_changed);
    Events::setActive(Event::map_changed);
}
