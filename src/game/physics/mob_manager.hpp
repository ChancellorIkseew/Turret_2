#pragma once
#include <CSP/centralized_ptr.hpp>
#include <array>
#include <vector>
#include "engine/render/sprite.hpp"
#include "id_manager.hpp"
#include "mob_ai.hpp"
#include "physics_base.hpp"

using MobID = uint16_t;
using Angle = float;

struct VisualPreset {
    csp::centralized_ptr<Texture> texture;
    const PixelCoord origin;
    const PixelCoord size;
};

struct MobPreset {
    const float maxSpeed;
    const float hitboxRadius;
    const MobMovingAI defaultMovingAI;
    const MobShootingAI defaultShootingAI;
    const VisualPreset visual;
};

struct MobSoA {
    std::vector<Hitbox> hitbox;
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> velocity;
    std::vector<Angle> angle;
    std::vector<csp::centralized_ptr<MobPreset>> preset;
    std::vector<MobID> id;
    std::vector<TeamID> teamID;
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
    t1_finline size_t getSoaIndexByMobID(const MobID mobID) { return soaIndexByMobID[mobID]; }
    t1_finline const MobSoA& getSoa() const noexcept { return soa; }
    t1_finline MobSoA& getSoa() noexcept { return soa; }
    //
    void reserve(const size_t capacity);
    void removeMob(size_t index);
    size_t addMob(
        const csp::centralized_ptr<MobPreset>& preset,
        const PixelCoord pos,
        const Angle angle,
        const TeamID teamID);
};
