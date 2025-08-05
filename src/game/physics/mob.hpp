#pragma once
#include <memory>
#include "engine/render/sprite.hpp"
#include "physics_base.hpp"

struct Mob;

struct MobAI {
    virtual ~MobAI() = default;
    virtual void operator()(Mob& mob) = 0;
};

struct MobPreset {
    std::string textureName;
    Explosion explosion;
    Health collisionDamage;
    Health maxHealth;
    float speed;
};

struct Mob {
    std::unique_ptr<MobAI> AI;
    const MobPreset& preset;
    Hitbox hitbox;
    PixelCoord position, velocity;
    Health health;
    float angle;
    bool wasted = false;
    TeamID teamID;

    Mob(std::unique_ptr<MobAI> AI, const MobPreset& preset, const PixelCoord position, const float angle, const TeamID teamID) :
        preset(preset), angle(angle), position(position), teamID(teamID),
        hitbox(position, position), health(preset.maxHealth), AI(std::move(AI)) { }
};
