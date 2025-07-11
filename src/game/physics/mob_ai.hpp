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
