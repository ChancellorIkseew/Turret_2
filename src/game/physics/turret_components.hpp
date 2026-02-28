#pragma once
#include <span>
#include "mob_manager.hpp"
#include "game/blocks/block_manager.hpp"

struct TurretComponents {
    std::span<PixelCoord> position;
    std::span<PixelCoord> velocity; // may be empty.
    std::span<TurretPresetID> preset;
    std::span<TeamID> teamID;
    std::span<ShootingData> shootingData;
    std::span<TickCount> restReloadTime;
    std::span<uint8_t> currentBarrel;
    std::span<AngleRad> turretAngle;
    size_t mobCount = 0;
};

inline TurretComponents fromBlocks(TurretSoA& src) {
    return {
        src.position,
        std::span<PixelCoord>(), // empty
        src.preset,
        src.teamID,
        src.shootingData,
        src.restReloadTime,
        src.currentBarrel,
        src.turretAngle,
        src.turretCount
    };
}

inline TurretComponents fromMobs(MobSoA& src) {
    return {
        src.position,
        src.velocity,
        src.turretPreset,
        src.teamID,
        src.shootingData,
        src.restReloadTime,
        src.currentBarrel,
        src.turretAngle,
        src.mobCount
    };
}
