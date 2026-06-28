#pragma once
#include <vector>
#include "engine/assets/preset_defs.hpp"

struct MobSoA {
    std::vector<float> hitboxRadius;
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> velocity;
    std::vector<AngleRad> angle;
    std::vector<Health> health;
    std::vector<Health> shieldHealth;
    std::vector<MobPresetID> preset;
    std::vector<TurretPresetID> turretPreset;
    std::vector<TeamID> teamID;
    std::vector<MotionData> motionData;
    std::vector<ShootingData> shootingData;
    std::vector<TickCount> restReloadTime;
    std::vector<uint8_t> currentBarrel;
    std::vector<AngleRad> turretAngle;
    std::vector<float> currentRecoil;
    std::vector<uint8_t> chassisFrame;
    std::vector<uint8_t> turretFrame;
    size_t mobCount = 0;
};

class MobManager {
    MobSoA soa;
public:
    MobManager(const size_t capacity) { reserve(capacity); }
    MobManager() = default;
    //
    t1_finline const MobSoA& getSoa() const noexcept { return soa; }
    t1_finline MobSoA& getSoa() noexcept { return soa; }
    //
    void reserve(const size_t capacity);
    void removeMob(const size_t targetIndex);
    void addMob(
        const MobPresetID preset,
        const TurretPresetID turretPreset,
        const PixelCoord position,
        const AngleRad angle,
        const Health health,
        const Health shieldHealth,
        const TeamID teamID,
        const float hitboxRadius,
        const MotionData motionData,
        const ShootingData shootingData,
        const TickCount restReloadTime,
        const AngleRad turretAngle);
};
