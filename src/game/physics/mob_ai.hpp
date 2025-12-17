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

struct BasicShootingAI : ShootingAI {
    ~BasicShootingAI() final = default;
    void update(const Mob& mob) final {
        
    }
};

struct PlayerControlledMoving : MovingAI {
    PlayerControlledMoving(PlayerController& playerController) :
        playerController(playerController) { }
    ~PlayerControlledMoving() final = default;
    void update(const Mob& mob) final {
        const auto vector = playerController.getMotionVector();
        if (vector == NO_MOTION)
            motionVector = NO_MOTION;
        else {
            motionAngle = atan2f(vector.x, vector.y);
            motionVector.x = sinf(motionAngle);
            motionVector.y = cosf(motionAngle);
        }
    }
private:
    const PlayerController& playerController;
};

struct PlayerControlledShooting : ShootingAI {
    PlayerControlledShooting(PlayerController& playerController) :
        playerController(playerController) {
    }
    ~PlayerControlledShooting() final = default;
    void update(const Mob& mob) final {
        aim = playerController.getAimCoord();
        firing = playerController.shootingActive();
    }
private:
    const PlayerController& playerController;
};
