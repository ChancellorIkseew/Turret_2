#include "particles_system.hpp"
//
#include "game/entities/particles_pool.hpp"

constexpr uint32_t COLOR_FADING = 0x00'00'00'08;

static inline void reduceLifeTime(ParticleSoA& soa) {
    for (auto& time : soa.restLifeTime) {
        --time;
    }
}

static inline void reduceColor(ParticleSoA& soa) {
    for (auto& color : soa.color) {
        color -= COLOR_FADING;
    }
}

static inline void move(ParticleSoA& soa) {
    for (size_t i = 0; i < soa.particleCount; ++i) {
        soa.position[i] += soa.facing[i] * soa.speed[i];
    }
}

static inline void cleanup(ParticlesPool& particlesPool) {
    const auto& soa = particlesPool.getSoa();
    // Reverse itaretion to avoid bugs with "swap and pop".
    for (size_t i = soa.particleCount; i > 0; --i) {
        size_t index = i - 1;
        if (soa.restLifeTime[index] <= 0)
            particlesPool.removeParticle(index);
    }
}

void particles::updateParticles(ParticlesPool& particles) {
    auto& soa = particles.getSoa();
    reduceLifeTime(soa);
    reduceColor(soa);
    move(soa);
    cleanup(particles);
}
