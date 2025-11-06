#pragma once
#include "engine/render/sprite.hpp"
#include "shell.hpp"
#include "game/presets/shell_presets.hpp"

class Team;

struct TurretPreset {
    const std::string textureName;
    const PixelCoord spriteOrigin;
    const PixelCoord shootPoint;
    const float minAngle, maxAngle;
    const ShellPreset& shell;
    const uint16_t reload;
};

static TurretPreset CTPreset("cannoner_bot", PixelCoord(22.5f, 28.0f), PixelCoord(22.5f, 28.0f), 0.f, 0.f, cannonShell, 0);

struct Turret {
    const TurretPreset& preset;
    float angle = 0;
    uint16_t restReloadTime = 0;
    PixelCoord position;
    TeamID teamID = 0;
    Turret(const TurretPreset& preset) : preset(preset) { }
    virtual ~Turret() = default;
    virtual void shoot(Team& team) = 0;
};
