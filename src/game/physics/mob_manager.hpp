#pragma once
#include <cassert>
#include <vector>
#include "engine/assets/preset_defs.hpp"
#include "id_manager.hpp"

using MobID = uint16_t;
using MobIndex = MobID;
class Presets;

struct MobSoA {
    std::vector<Hitbox> hitbox;
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> velocity;
    std::vector<AngleRad> angle;
    std::vector<Health> health;
    std::vector<MobPresetID> preset;
    std::vector<MobID> id;
    std::vector<TeamID> teamID;
    std::vector<MotionData> motionData;
    std::vector<ShootingData> shootingData;
    std::vector<TickCount> restReloadTime;
    std::vector<uint8_t> currentBarrel;
    std::vector<AngleRad> turretAngle;
    std::vector<uint8_t> chassisFrame;
    std::vector<uint8_t> turretFrame;
    size_t mobCount = 0;
};

class MobManager {
    std::array<MobIndex, IDManager<MobID>::MAX_ID> soaIndexByMobID;
    IDManager<MobID> idManager = IDManager<MobID>(512);
    MobSoA soa;
    void fillIndexes();
public:
    MobManager(const size_t capacity) { fillIndexes(); reserve(capacity); }
    MobManager() { fillIndexes(); }
    //
    t1_finline const MobSoA& getSoa() const noexcept { return soa; }
    t1_finline MobSoA& getSoa() noexcept { return soa; }
    t1_finline size_t getSoaIndexByMobID(const MobID mobID) const noexcept {
        assert(mobID != IDManager<MobID>::INVALID_ID);
        assert(isIndexValid(soaIndexByMobID[mobID]));
        return soaIndexByMobID[mobID];
    }
    t1_finline bool isIndexValid(const size_t index) const noexcept {
        return index < soa.mobCount;
    }
    //
    void reserve(const size_t capacity);
    void removeMob(const size_t targetIndex);
    MobID addMob(
        const Presets& presets,
        const MobPresetID preset,
        const PixelCoord position,
        const AngleRad angle,
        const Health health,
        const TeamID teamID,
        const MotionData motionData,
        const ShootingData shootingData,
        const TickCount restReloadTime,
        const AngleRad turretAngle);
};
