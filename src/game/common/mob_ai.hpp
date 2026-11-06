#pragma once
#include "engine/assets/typedefs.hpp"
#include "engine/coords/pixel_coord.hpp"

using HeatMapID = uint16_t;

struct MotionData {
    MovingAI aiType = MovingAI::basic;
    HeatMapID heatmapID; // Used only by mobs with pathfinding.
    PixelCoord target; 
};

struct ShootingData {
    ShootingAI aiType = ShootingAI::basic;
    bool hasTarget = false;
    PixelCoord target;
};
