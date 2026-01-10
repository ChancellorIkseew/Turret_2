#pragma once
#include <CSP/centralized_ptr.hpp>
#include <vector>
#include "engine/render/texture.hpp"
#include "physics_base.hpp"

class Presets;

struct ShellVisualPreset {
    Texture texture;
    PixelCoord origin;
    PixelCoord size;
};

struct ShellPreset {
    float speed;
    Health damage;
    TickCount maxLifeTime;
    Explosion explosion;
    ShellVisualPreset visual;
};

struct ShellSoA {
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> velocity;
    std::vector<AngleRad> angle;
    std::vector<Health> restDamage;
    std::vector<TickCount> restLifeTime;
    std::vector<PresetID> preset;
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
        const Presets& presets,
        const PresetID preset,
        const PixelCoord position,
        const AngleRad angle,
        const Health restDamage,
        const TickCount restLifeTime,
        const TeamID teamID);
};
