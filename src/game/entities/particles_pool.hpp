#pragma once
#include <vector>
#include "engine/coords/pixel_coord.hpp"
#include "game/common/physics_base.hpp"

enum class PType : uint8_t { light, smoke, shard };

struct ParticleSoA {
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> facing;
    std::vector<PixelCoord> size;
    std::vector<AngleRad> angle;
    std::vector<float> speed;
    std::vector<uint32_t> color;
    std::vector<uint32_t> colorFading;
    std::vector<TickCount> restLifeTime;
    std::vector<PType> type;
    size_t particleCount = 0;
};

class ParticlesPool {
    ParticleSoA soa;
public:
    ParticlesPool(const size_t capacity) { reserve(capacity); }
    ParticlesPool() = default;
    //
    t1_finline const ParticleSoA& getSoa() const noexcept { return soa; }
    t1_finline ParticleSoA& getSoa() noexcept { return soa; }
    //
    void reserve(const size_t capacity);
    void removeParticle(const size_t index);
    void addParticle(
        const PixelCoord position,
        const PixelCoord size,
        const float angle,
        const float speed,
        const uint32_t color,
        const uint32_t colorFading,
        const TickCount restLifeTime,
        const PType type);
};
