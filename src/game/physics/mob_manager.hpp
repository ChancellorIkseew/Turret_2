#pragma once
#include <CSP/centralized_ptr.hpp>
#include <array>
#include <cassert>
#include <vector>
#include "engine/render/sprite.hpp"
#include "id_manager.hpp"
#include "mob_ai.hpp"
#include "physics_base.hpp"
#include "shell_manager.hpp"

using MobID = uint16_t;

struct MobVisualPreset {
    csp::centralized_ptr<Texture> texture;
    const PixelCoord origin;
    const PixelCoord size;
};

struct TurretVisualPreset {
    csp::centralized_ptr<Texture> texture;
    const PixelCoord origin;
    const PixelCoord size;
};

struct TurretPreset {
    const TickCount reload;
    const AngleRad rotationSpeed;
    const size_t barrelsCount;
    const std::array<PixelCoord, 4> barrels;
    csp::centralized_ptr<ShellPreset> shell;
    const TurretVisualPreset visual;
};

struct MobPreset {
    const float maxSpeed;
    const float hitboxRadius;
    const Health maxHealth;
    const MovingAI defaultMovingAI;
    const ShootingAI defaultShootingAI;
    csp::centralized_ptr<TurretPreset> turret;
    const MobVisualPreset visual;
};

struct MobSoA {
    std::vector<Hitbox> hitbox;
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> velocity;
    std::vector<AngleRad> angle;
    std::vector<Health> health;
    std::vector<csp::centralized_ptr<MobPreset>> preset;
    std::vector<MobID> id;
    std::vector<TeamID> teamID;
    std::vector<MotionData> motionData;
    std::vector<ShootingData> shootingData;
    std::vector<TickCount> restReloadTime;
    std::vector<uint8_t> currentBarrel;
    std::vector<AngleRad> turretAngle;
    size_t mobCount = 0;
};

class MobManager {
    std::array<MobID, IDManager<MobID>::MAX_ID> soaIndexByMobID;
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
        return soaIndexByMobID[mobID];
    }
    //
    void reserve(const size_t capacity);
    void removeMob(const size_t index);
    MobID addMob(
        const csp::centralized_ptr<MobPreset>& preset,
        const PixelCoord position,
        const AngleRad angle,
        const Health health,
        const TeamID teamID,
        const MotionData motionData,
        const ShootingData shootingData,
        const TickCount restReloadTime,
        const AngleRad turretAngle);
};
