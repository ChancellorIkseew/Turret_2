#pragma once
#include "mob.hpp"

struct Rocket : MobAI {
    static constexpr float DETONATION_DISTANCE = 20.0f;
    std::list<Mob>& mobs;
    ~Rocket() final = default;
    void operator()(Mob& mob) final {
        for (const auto& enemy : mobs) {
            const auto distance = enemy.position - mob.position;
            if (abs(distance.x) < DETONATION_DISTANCE && abs(distance.y) < DETONATION_DISTANCE)
                mob.wasted = true;
        }
    }
};

struct Hoaming : MobAI {
    ~Hoaming() final = default;
    void operator()(Mob& mob) final {

    }
};

struct MovementAI {
    virtual ~MovementAI() = default;
    virtual void operator()(Mob& mob) = 0;
};

struct FolowingAI : MovementAI {
    Mob* target;
    ~FolowingAI() final = default;
    void operator()(Mob& mob) final {

    }
};

#include "game/player/player_controller.hpp"

struct PlayerControlled : MobAI {
    ~PlayerControlled() final = default;
    void operator()(Mob& mob) final {
        MobController::getMotionVector();
        const auto vector = MobController::getMotionVector();
        if (vector == PixelCoord(0.0f, 0.0f))
            mob.velocity = PixelCoord(0.0f, 0.0f);
        else {
            const float motionAngle = atan2f(vector.x, vector.y);
            mob.velocity.x = sin(motionAngle) * mob.preset.speed;
            mob.velocity.y = cos(motionAngle) * mob.preset.speed;
        }
    }
};
