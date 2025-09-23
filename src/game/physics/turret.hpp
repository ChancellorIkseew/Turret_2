#pragma once
#include "engine/render/sprite.hpp"
#include "shell.hpp"

class Team;

struct TurretPreset {
    const std::string textureName;
    const PixelCoord spriteOrigin;
    const PixelCoord shootPoint;
    const float minAngle, maxAngle;
    const ShellPreset& shell;
    const uint16_t reload;
};

struct Turret {
    const TurretPreset& preset;
    float angle;
    uint16_t restReloadTime;
    PixelCoord position;
    TeamID teamID;

    void shoot(Team& team);
};
