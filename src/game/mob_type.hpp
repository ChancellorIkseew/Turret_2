#pragma once
#include "render/sprite.hpp"

struct MobType {
    const Texture texture;
    MobType(const std::filesystem::path& path) : texture(path) { }
};


class Mob {
    Sprite sprite;
    PixelCoord coord;
public:
    Mob(const MobType& type) : sprite(type.texture) { }

    void draw() {
        sprite.setPosition(coord);
        sprite.draw();
    }

    void setPixelCoord(const PixelCoord coord) {
        this->coord = coord;
    }
};


