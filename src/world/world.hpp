#pragma once
#include <vector>
#include "render/sprite.hpp"
#include "world/camera.hpp"
#include <iostream>

class World {
    std::vector<std::vector<int>> terrain;
    Texture texture;
    Sprite sprite;
public:
    World() : texture("res/images/icon.bmp"), sprite(texture) {
        for (int x = 0; x < 10; ++x) {
            std::vector<int> line;
            for (int y = 0; y < 10; ++y) {
                line.push_back(0);
            }
            terrain.push_back(line);
        }
        terrain[1][2] = 1;
        terrain[8][6] = 1;
    }
    
    void print() {
        for (const auto& line : terrain) {
            for (const int& y : line) {
                std::cout << y;
            }
            std::cout << '\n';
        }
    }

    void draw(const Camera& camera) {
        for (int x = 0; x < 10; ++x) {
            for (int y = 0; y < 10; ++y) {
                if (terrain[x][y] != 0)
                    continue;
                sprite.setPosition(PixelCoord(x, y) * 100 - camera.getPosition());
                sprite.drawFast();
            }
        }
    }

};
