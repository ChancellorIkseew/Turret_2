#pragma once
#include "render/sprite.hpp"

struct MobType {
    //const Texture texture;
    //MobType(const std::filesystem::path& path) : texture(path) { }
};


class Mob {
    Sprite sprite;
    PixelCoord coord;
    float angleDeg = 0.0f;
public:
    Mob(const MobType& type) : sprite("cannoner_bot") { }

    void draw() {
        sprite.setOrigin(16, 16);
        sprite.setRotation(angleDeg);
        sprite.setPosition(coord);
        sprite.draw();
    }

    void setPixelCoord(const PixelCoord coord) {
        this->coord = coord;
    }
    void setAngleDeg(float angleDeg) {
        this->angleDeg = angleDeg;
    }
};
