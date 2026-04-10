#pragma once
#include <vector>
#include "engine/assets/preset_defs.hpp"

class Presets;

struct TurretSoA {
    std::vector<PixelCoord> position;
    std::vector<AngleRad> angle;
    std::vector<TurretPresetID> preset;
    std::vector<TeamID> teamID;
    std::vector<ShootingData> shootingData;
    std::vector<TickCount> restReloadTime;
    std::vector<uint8_t> currentBarrel;
    std::vector<uint8_t> turretFrame;
    size_t turretCount = 0;
};

class TurretManager {
    TurretSoA soa;
    void fillIndexes();
public:
    TurretManager(const size_t capacity) { reserve(capacity); }
    //
    t1_finline const TurretSoA& getSoa() const noexcept { return soa; }
    t1_finline TurretSoA& getSoa() noexcept { return soa; }
    //
    void reserve(const size_t capacity);
    void removeTurret(const size_t targetIndex);
    void addTurret(
        const TurretPresetID preset,
        const PixelCoord position,
        const AngleRad angle,
        const TeamID teamID,
        const ShootingData shootingData,
        const TickCount restReloadTime);
};
