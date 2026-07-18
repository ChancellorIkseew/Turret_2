#pragma once
#include "engine/render/renderer.hpp"
#include "game/entities/particles_pool.hpp"
#include "game/player/camera.hpp"

inline void drawLightParticles(const Camera& camera, Renderer& renderer, const ParticleSoA& soa) {
    const size_t particleCount = soa.particleCount;
    if (particleCount == 0)
        return;
    constexpr TextureRect rect{ 0.f, 0.f, 1.f, 1.f };
    for (size_t i = 0; i < particleCount; ++i) {
        if (soa.type[i] == PType::light && camera.contains(soa.position[i]))
            renderer.draw(rect, soa.position[i], soa.size[i], soa.size[i] / 2, t1::PI - soa.angle[i], soa.color[i]);
    }
}

// Same with light but uses other shader
inline void drawSmokeParticles(const Camera& camera, Renderer& renderer, const ParticleSoA& soa) {
    const size_t particleCount = soa.particleCount;
    if (particleCount == 0)
        return;
    constexpr TextureRect rect{ 0.f, 0.f, 1.f, 1.f };
    for (size_t i = 0; i < particleCount; ++i) {
        if (soa.type[i] == PType::smoke && camera.contains(soa.position[i]))
            renderer.draw(rect, soa.position[i], soa.size[i], soa.size[i] / 2, t1::PI - soa.angle[i], soa.color[i]);
    }
}

inline void drawShardParticles(const Camera& camera, Renderer& renderer, const ParticleSoA& soa) {
    const size_t particleCount = soa.particleCount;
    if (particleCount == 0)
        return;
    for (size_t i = 0; i < particleCount; ++i) {
        if (soa.type[i] == PType::shard && camera.contains(soa.position[i]))
            renderer.drawRect(soa.position[i], soa.size[i], soa.size[i] / 2, t1::PI - soa.angle[i], soa.color[i]);
    }
}
