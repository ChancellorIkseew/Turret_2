#pragma once
#include "engine/render/sprite.hpp"
#include "physics_base.hpp"

struct Mob;

struct MobAI {
    virtual ~MobAI() = default;
    virtual void operator()(Mob& mob) = 0;
};

struct MobPreset {
    MobAI* AI;
    Sprite sprite;
    Explosion explosion;
    Health collisionDamage;
    Health maxHealth;
    float speed;
};

struct Mob {
    const MobPreset& preset;
    Hitbox hitbox;
    PixelCoord position, velocity;
    Health health;
    float angle;
    bool wasted = false;
    TeamID teamID;

    Mob(const MobPreset& preset, const PixelCoord position, const float angle, const TeamID teamID) :
        preset(preset), angle(angle), position(position), teamID(teamID),
        hitbox(position, position), health(preset.maxHealth) { }
};
