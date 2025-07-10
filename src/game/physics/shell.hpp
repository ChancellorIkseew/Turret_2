#pragma once
#include "engine/render/sprite.hpp"
#include "physics_base.hpp"

struct ShellPreset {
    Sprite sprite;
    Explosion explosion;
    Health directDamage;
    LifeTime maxLifeTime;
    float speed;
};

struct Shell {
    const ShellPreset& preset;
    PixelCoord position, velocity;
    float angle;
    LifeTime restLifeTime;
    bool wasted = false;

    Shell(const ShellPreset& preset, const PixelCoord position, const float angle) :
        preset(preset), angle(angle), restLifeTime(preset.maxLifeTime),
        position(position), velocity(/*by angle and speed*/) { }
};
