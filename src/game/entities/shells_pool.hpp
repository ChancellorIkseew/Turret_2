#pragma once
#include <vector>
#include "engine/assets/preset_defs.hpp"

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

class ShellsPool {
    ShellSoA soa;
public:
    ShellsPool(const size_t capacity) { reserve(capacity); }
    ShellsPool() = default;
    //
    t1_finline const ShellSoA& getSoa() const noexcept { return soa; }
    t1_finline ShellSoA& getSoa() noexcept { return soa; }
    //
    void reserve(const size_t capacity);
    void removeShell(const size_t index);
    void addShell(
        const ShellPresetID preset,
        const PixelCoord position,
        const PixelCoord velocity,
        const AngleRad angle,
        const Health restDamage,
        const TickCount restLifeTime,
        const TeamID teamID);
};
