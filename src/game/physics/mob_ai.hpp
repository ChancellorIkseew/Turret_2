#pragma once
#include "mob.hpp"
#include "game/player/player_controller.hpp"

struct BasicMovingAI : MovingAI {
    ~BasicMovingAI() final = default;
    void update(const Mob& mob) final {
        const float motionAngleRad = atan2f(dest.x - mob.position.x, dest.y - mob.position.y);
        motionVector.x = sinf(motionAngleRad);
        motionVector.y = cosf(motionAngleRad);
    } 
};

struct PlayerControlledMoving : MovingAI {
    ~PlayerControlledMoving() final = default;
    void update(const Mob& mob) final {
        MobController::getMotionVector();
        const auto vector = MobController::getMotionVector();
        if (vector == PixelCoord(0.0f, 0.0f))
            motionVector = PixelCoord(0.0f, 0.0f);
        else {
            const float motionAngleRad = atan2f(vector.x, vector.y);
            motionVector.x = sin(motionAngleRad);
            motionVector.y = cos(motionAngleRad);
        }
    }
};
