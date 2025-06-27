#include "world.hpp"
//
#include <iostream>
#include "game/generation/generation.hpp"

World::World(const TileCoord mapSize) : terrain(mapSize.x)  {
    for (auto& line : terrain) {
        line.resize(mapSize.y);
    }
    gen::generate(terrain, mapSize);

    terrain[10][10].block = new Block("round_shadow", "ice_block", TileCoord(10, 10));
}

void World::print() {
    for (int y = 0; y < terrain.size(); ++y) {
        for (int x = 0; x < terrain[0].size(); ++x) {
            std::cout << static_cast<int>(terrain[x][y].tileType);
        }
        std::cout << '\n';
    }
}

void World::placeTile(const TileCoord tile, const TileType tileType) {
    terrain[tile.x][tile.y].tileType = tileType;
}
