#pragma once
#include <cstdint>
#include <string>
#include "engine/coords/pixel_coord.hpp"

enum class MovingAI : uint8_t {
    none,
    basic,
    player_controlled
};

enum class ShootingAI : uint8_t {
    none,
    basic,
    player_controlled
};

using HeatMapID = uint16_t;

struct MotionData {
    MovingAI aiType;
    HeatMapID heatmapID; // Used only by mobs with pathfinding.
    PixelCoord target; 
};

struct ShootingData {
    ShootingAI aiType;
    bool isShooting;
    PixelCoord target;
};

namespace mob_ai{
    MovingAI getMovingAI(const std::string& name);
    ShootingAI getShootingAI(const std::string& name);
}
