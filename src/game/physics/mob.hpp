#pragma once
#include <memory>
#include "engine/render/sprite.hpp"
#include "physics_base.hpp"
#include "turret.hpp"

struct Mob;

struct MovingAI {
protected:
    PixelCoord dest, motionVector;
    float motionAngle = 0.0f;
public:
    virtual ~MovingAI() = default;
    virtual void update(const Mob& mob) = 0;
    PixelCoord getDest() const { return dest; }
    PixelCoord getMotionVector() const { return motionVector; }
    float getMotionAngle() const { return motionAngle; }
    void setDest(const PixelCoord dest) {
        this->dest = dest;
    }
};

struct ShootingAI {
protected:
    PixelCoord aim;
public:
    virtual ~ShootingAI() = default;
    //
    virtual void update(const Mob& mob) = 0;
    //
    PixelCoord getAim() { return aim; }
    void setAim(const PixelCoord aim) {
        this->aim = aim;
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
    std::unique_ptr<Turret> turret;
    const MobPreset& preset;
    Sprite sprite;
    Hitbox hitbox;
    PixelCoord position, velocity;
    Health health;
    float angle;
    bool wasted = false, colided = false;
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
