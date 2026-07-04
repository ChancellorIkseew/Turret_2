#pragma once
#include "engine/render/renderer.hpp"
#include "game/entities/particles_pool.hpp"
#include "game/player/camera.hpp"

constexpr PixelCoord PARTICLE_SIZE(15.f, 15.f);
constexpr PixelCoord PARTICLE_ORIGIN(7.5f, 7.5f);

void drawParticles(const Camera& camera, Renderer& renderer, const ParticleSoA& soa) {
    const size_t particleCount = soa.particleCount;
    if (particleCount == 0)
        return;
    constexpr TextureRect rect{ 0.f, 0.f, 1.f, 1.f };

    for (size_t i = 0; i < particleCount; ++i) {
        if (camera.contains(t1::tile(soa.position[i])))
            renderer.draw(rect, soa.position[i], PARTICLE_SIZE, PARTICLE_ORIGIN, 0.f, soa.color[i]);
    }
}
