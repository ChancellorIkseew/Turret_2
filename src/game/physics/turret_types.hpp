#pragma once
#include "turret.hpp"
#include "team/team.hpp"

struct CannonTurret : Turret {
    CannonTurret(const TurretPreset& preset) : Turret(preset) { }
    ~CannonTurret() final = default;
    //
    void shoot(Team& team) final {
        team.spawnShell(preset.shell, position, angle);
    }
};
