#include "world.hpp"
//
#include <iostream>
#include "game/generation/generation.hpp"

World::World(const TileCoord mapSize) : terrain(mapSize.x)  {
    for (auto& line : terrain) {
        line.resize(mapSize.y);
    }
    gen::generate(terrain, mapSize);
}

void World::print() {
    for (int y = 0; y < terrain.size(); ++y) {
        for (int x = 0; x < terrain[0].size(); ++x) {
            std::cout << static_cast<int>(terrain[x][y].tileType);
        }
        std::cout << '\n';
    }
}
