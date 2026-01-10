#pragma once
#include <CSP/centralized_ptr.hpp>
#include <array>
#include <cassert>
#include <vector>
#include "engine/render/texture.hpp"
#include "id_manager.hpp"
#include "mob_ai.hpp"
#include "physics_base.hpp"
#include "shell_manager.hpp"

using MobID = uint16_t;

struct MobVisualPreset {
    Texture texture;
    PixelCoord origin;
    PixelCoord size;
    uint8_t frameTicks;
    float frameHeight;
    uint8_t frameCount;
    std::array<uint8_t, 16> frameOrder;
};

struct TurretVisualPreset {
    Texture texture;
    PixelCoord origin;
    PixelCoord size;
    uint8_t frameCount;
};

struct TurretPreset {
    TickCount reload;
    AngleRad rotationSpeed;
    size_t barrelsCount;
    std::array<PixelCoord, 4> barrels;
    PresetID shell;
    TurretVisualPreset visual;
};

struct MobPreset {
    float maxSpeed;
    float hitboxRadius;
    Health maxHealth;
    MovingAI defaultMovingAI;
    ShootingAI defaultShootingAI;
    PresetID turret;
    MobVisualPreset visual;
};

struct MobSoA {
    std::vector<Hitbox> hitbox;
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> velocity;
    std::vector<AngleRad> angle;
    std::vector<Health> health;
    std::vector<PresetID> preset;
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
        const Presets& presets,
        const PresetID preset,
        const PixelCoord position,
        const AngleRad angle,
        const Health health,
        const TeamID teamID,
        const MotionData motionData,
        const ShootingData shootingData,
        const TickCount restReloadTime,
        const AngleRad turretAngle);
};
