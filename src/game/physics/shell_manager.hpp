#pragma once
#include <CSP/centralized_ptr.hpp>
#include <vector>
#include "engine/render/sprite.hpp"
#include "physics_base.hpp"

struct ShellVisualPreset {
    csp::centralized_ptr<Texture> texture;
    const PixelCoord origin;
    const PixelCoord size;
};

struct ShellPreset {
    const float speed;
    const Health damage;
    const TickCount maxLifeTime;
    const Explosion explosion;
    const ShellVisualPreset visual;
};

struct ShellSoA {
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> velocity;
    std::vector<AngleRad> angle;
    std::vector<Health> restDamage;
    std::vector<TickCount> restLifeTime;
    std::vector<csp::centralized_ptr<ShellPreset>> preset;
    std::vector<TeamID> teamID;
    size_t shellCount = 0;
};

class ShellManager {
    ShellSoA soa;
public:
    ShellManager(const size_t capacity) { reserve(capacity); }
    ShellManager() = default;
    //
    t1_finline const ShellSoA& getSoa() const noexcept { return soa; }
    t1_finline ShellSoA& getSoa() noexcept { return soa; }
    //
    void reserve(const size_t capacity);
    void removeShell(const size_t index);
    void addShell(
        const csp::centralized_ptr<ShellPreset>& preset,
        const PixelCoord position,
        const AngleRad angle,
        const Health restDamage,
        const TickCount restLifeTime,
        const TeamID teamID);
};
