#pragma once
#include <cstdint>
#include <string>

enum class MobMovingAI : uint8_t {
    none,
    basic,
    player_controlled
};

enum class MobShootingAI : uint8_t {
    none,
    basic,
    player_controlled
};

namespace mob_ai{
    MobMovingAI getMovingAI(const std::string& name);
    MobShootingAI getShootingAI(const std::string& name);
}
