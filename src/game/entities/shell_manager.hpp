#pragma once
#include <vector>
#include "engine/assets/preset_defs.hpp"

class Presets;

struct ShellSoA {
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> velocity;
    std::vector<AngleRad> angle;
    std::vector<Health> restDamage;
    std::vector<TickCount> restLifeTime;
    std::vector<ShellPresetID> preset;
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
        const ShellPresetID preset,
        const PixelCoord position,
        const AngleRad angle,
        const Health restDamage,
        const TickCount restLifeTime,
        const TeamID teamID);
};
