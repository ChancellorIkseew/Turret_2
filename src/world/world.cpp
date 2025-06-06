#include "world.hpp"
//
#include "world/camera.hpp"
#include <iostream>

World::World() : texture("res/images/icon.bmp"), sprite(texture) {
    for (int x = 0; x < 100; ++x) {
        std::vector<int> line;
        for (int y = 0; y < 100; ++y) {
            line.push_back(0);
        }
        terrain.push_back(line);
    }
    terrain[1][2] = 1;
    terrain[8][6] = 1;
}

void World::print() {
    for (const auto& line : terrain) {
        for (const int& y : line) {
            std::cout << y;
        }
        std::cout << '\n';
    }
}

void World::draw(const Camera& camera) {
    const TileCoord start = camera.getStartTile();
    const TileCoord end = camera.getEndTile();
    for (int x = start.x; x < end.x; ++x) {
        for (int y = start.y; y < end.y; ++y) {
            if (x == y)
                continue;
            sprite.setPosition(PixelCoord(x, y) * 32 - camera.getPosition());
            sprite.drawFast();
        }
    }
}
