#pragma once
#include <vector>
#include "engine/coords/pixel_coord.hpp"
#include "game/physics/physics_base.hpp"

class ParticlesSoA {
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> velocity;
    std::vector<AngleRad> angle;
    std::vector<uint32_t> color;
    std::vector<TickCount> restLifeTime;
    size_t particleCount = 0;
};

class ParticlesManager {
    ParticlesSoA soa;
public:
    ParticlesManager(const size_t capacity) { reserve(capacity); }
    ParticlesManager() = default;
    //
    t1_finline const ParticlesSoA& getSoa() const noexcept { return soa; }
    t1_finline ParticlesSoA& getSoa() noexcept { return soa; }
    //
    void reserve(const size_t capacity);
    void removeParticle(const size_t index);
    void addParticle();
};
