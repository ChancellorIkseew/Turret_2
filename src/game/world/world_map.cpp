#include "world_map.hpp"
//
#include <iostream>
#include "game/generation/generation.hpp"

WorldMap::WorldMap(const TileCoord mapSize) : terrain(mapSize.x), mapSize(mapSize) {
    content::load(registry);
    for (auto& line : terrain) {
        line.resize(mapSize.y);
    }
    gen::generate(terrain, mapSize);

    terrain[10][10].block = new Block("round_shadow", "ice_block", TileCoord(10, 10));
}

void WorldMap::print() {
    for (int y = 0; y < terrain.size(); ++y) {
        for (int x = 0; x < terrain[0].size(); ++x) {
            std::cout << static_cast<int>(terrain[x][y].floor);
        }
        std::cout << '\n';
    }
}

void WorldMap::placeFloor(const TileCoord tile, const uint8_t floorID) {
    if (tileExists(tile))
        terrain[tile.x][tile.y].floor = floorID;
}
void WorldMap::placeOverlay(const TileCoord tile, const uint8_t overlayID) {
    if (tileExists(tile))
        terrain[tile.x][tile.y].overlay = overlayID;
}