#include "world_map.hpp"
//
#include "game/events/events.hpp"

WorldMap::WorldMap(const TileCoord mapSize, std::vector<uint8_t>& floor, std::vector<OrePresetID>& ore) :
    mapSize(mapSize), floor(std::move(floor)), ore(std::move(ore)) { }

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
void WorldMap::placeOverlay(const TileCoord tile, const OrePresetID orePresetID) {
    if (!tileExists(tile))
        return;
    ore[getIndex(tile.x, tile.y, mapSize)] = orePresetID;
    Events::setActive(Event::overlay_changed);
    Events::setActive(Event::map_changed);
}
