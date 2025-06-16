#include "world.hpp"
//
#include <iostream>
#include "camera.hpp"
#include "game/generation/generation.hpp"

World::World(const TileCoord mapSize) : sprite("icon"), terrain(mapSize.x)  {
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

void World::draw(const Camera& camera) {
    const TileCoord start = camera.getStartTile();
    const TileCoord end = camera.getEndTile();
    for (int x = start.x; x < end.x; ++x) {
        for (int y = start.y; y < end.y; ++y) {
            switch (terrain[x][y].tileType) {
            case TileType::SAND: sprite.setTexture(Texture("sand")); break;
            case TileType::GRASS: sprite.setTexture(Texture("grass")); break;
            case TileType::WATER_SHALLOW: sprite.setTexture(Texture("water")); break;
            }
            sprite.setPosition(PixelCoord(x, y) * 32 - camera.getPosition());
            sprite.drawFast();
        }
    }
}
