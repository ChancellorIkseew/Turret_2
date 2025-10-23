#pragma once
#include "mob.hpp"
#include "game/player/player_controller.hpp"

constexpr PixelCoord NO_MOTION(0.0f, 0.0f);

struct BasicMovingAI : MovingAI {
    ~BasicMovingAI() final = default;
    void update(const Mob& mob) final {
        if (t1::areCloser(dest, mob.position, mob.preset.speed))
            motionVector = NO_MOTION;
        else {
            motionAngle = atan2f(dest.x - mob.position.x, dest.y - mob.position.y);
            motionVector.x = sinf(motionAngle);
            motionVector.y = cosf(motionAngle);
        }
    }
};

struct PlayerControlledMoving : MovingAI {
    ~PlayerControlledMoving() final = default;
    void update(const Mob& mob) final {
        const auto vector = MobController::getMotionVector();
        if (vector == NO_MOTION)
            motionVector = NO_MOTION;
        else {
            motionAngle = atan2f(vector.x, vector.y);
            motionVector.x = sinf(motionAngle);
            motionVector.y = cosf(motionAngle);
        }
    }
};

struct PlayerControlledShooting : ShootingAI {
    ~PlayerControlledShooting() final = default;
    void update(const Mob& mob) final {
        aim = MobController::getAimCoord();
        firing = MobController::shootingActive();
    }
};
