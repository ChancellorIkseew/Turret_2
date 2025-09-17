#pragma once
#include <memory>
#include "engine/render/sprite.hpp"
#include "physics_base.hpp"

struct Mob;

struct MovingAI {
protected:
    PixelCoord dest, motionVector;
    float motionAngleRad;
public:
    virtual ~MovingAI() = default;
    virtual void update(const Mob& mob) = 0;
    PixelCoord getDest() const { return dest; }
    PixelCoord getMotionVector() const { return motionVector; }
    float getMotionAngleRad() const { return motionAngleRad; }
    void setDest(const PixelCoord dest) {
        this->dest = dest;
    }
};

struct ShootingAI {
protected:
    PixelCoord dest;
    PixelCoord motionVector;
public:
    virtual ~ShootingAI() = default;
    //
    virtual void update(const Mob& mob) = 0;
    //
    void setDest(const PixelCoord dest) {
        this->dest = dest;
    }
};

struct MobPreset {
    const std::string textureName;
    const PixelCoord spriteOrigin;
    const Explosion explosion;
    const Health collisionDamage;
    const Health maxHealth;
    const float speed;
    const float hitboxRadius;
};

struct Mob {
    std::unique_ptr<MovingAI> movingAI;
    std::unique_ptr<ShootingAI> shootingAI;
    const MobPreset& preset;
    Sprite sprite;
    Hitbox hitbox;
    PixelCoord position, velocity;
    Health health;
    float angle;
    bool wasted = false;
    TeamID teamID;
    //
    Mob(std::unique_ptr<MovingAI> movingAI,
        std::unique_ptr<ShootingAI> shootingAI,
        const MobPreset& preset,
        const PixelCoord position,
        const float angle,
        const TeamID teamID) :
        movingAI(std::move(movingAI)),
        shootingAI(std::move(shootingAI)),
        preset(preset),
        sprite(preset.textureName),
        hitbox(position, preset.hitboxRadius),
        position(position),
        angle(angle),
        teamID(teamID),
        health(preset.maxHealth) {
        sprite.setOrigin(preset.spriteOrigin);
    }
};
