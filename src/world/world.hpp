#pragma once
#include <vector>
#include "render/sprite.hpp"

class Camera;

class World {
    std::vector<std::vector<int>> terrain;
    Texture texture;
    Sprite sprite;
public:
    World();
    void print();
    void draw(const Camera& camera);
};
