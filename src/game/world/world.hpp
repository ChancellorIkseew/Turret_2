#pragma once
#include <vector>
#include "engine/render/sprite.hpp"

class Camera;

class World {
    std::vector<std::vector<int>> terrain;
    Sprite sprite;
public:
    World();
    void print();
    void draw(const Camera& camera);
};
