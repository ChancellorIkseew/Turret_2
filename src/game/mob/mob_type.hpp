#pragma once
#include "engine/render/sprite.hpp"

struct MobType {
    const std::string texture;
    const PixelCoord origin;
    const PixelCoord hitbox;
    const float speed;
    const Sint32 maxHealth;
};

constexpr MobType CANNON_BOSS("cannon_boss", PixelCoord(22.5f, 22.5f), PixelCoord(40.0f, 40.0f), 0.1f, 270);

class Mob {
    Sprite sprite;
    PixelCoord coord;
    PixelCoord hitbox;
    Sint32 health;
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
