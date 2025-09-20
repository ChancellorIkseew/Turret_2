#pragma once
#include "engine/coords/transforms.hpp"
#include "engine/render/sprite.hpp"
#include "physics_base.hpp"

struct ShellPreset {
    const std::string textureName;
    const PixelCoord spriteOrigin;
    const Explosion explosion;
    const Health directDamage;
    const LifeTime maxLifeTime;
    const float speed;
};

struct Shell {
    const ShellPreset& preset;
    Sprite sprite;
    PixelCoord position, velocity;
    float angle;
    LifeTime restLifeTime;
    bool wasted = false;
    TeamID teamID;

    Shell(const ShellPreset& preset,
        const PixelCoord position,
        const float angle,
        const TeamID teamID) :
        preset(preset),
        sprite(preset.textureName),
        position(position),
        velocity(sinf(angle) * preset.speed, cosf(angle) * preset.speed),
        angle(angle),
        restLifeTime(preset.maxLifeTime),
        teamID(teamID) {
        sprite.setOrigin(preset.spriteOrigin);
    }
};
