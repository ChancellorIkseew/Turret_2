#pragma once
#include <vector>
#include "engine/coords/pixel_coord.hpp"
#include "game/physics/physics_base.hpp"

struct ParticleSoA {
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> facing;
    std::vector<AngleRad> speed;
    std::vector<uint32_t> color;
    std::vector<TickCount> restLifeTime;
    size_t particleCount = 0;
};

class ParticleManager {
    ParticleSoA soa;
public:
    ParticleManager(const size_t capacity) { reserve(capacity); }
    ParticleManager() = default;
    //
    t1_finline const ParticleSoA& getSoa() const noexcept { return soa; }
    t1_finline ParticleSoA& getSoa() noexcept { return soa; }
    //
    void reserve(const size_t capacity);
    void removeParticle(const size_t index);
    void addParticle(
        const PixelCoord position,
        const PixelCoord facing,
        const float speed,
        const uint32_t color,
        const TickCount restLifeTime);
    void addParticle(
        const PixelCoord position,
        const float angle,
        const float speed,
        const uint32_t color,
        const TickCount restLifeTime);
};
