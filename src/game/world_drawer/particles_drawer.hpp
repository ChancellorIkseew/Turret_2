#pragma once
#include "engine/render/renderer.hpp"
#include "game/entities/particles_pool.hpp"
#include "game/player/camera.hpp"

void drawParticles(const Camera& camera, Renderer& renderer, const ParticleSoA& soa) {
    const size_t particleCount = soa.particleCount;
    if (particleCount == 0)
        return;
    constexpr TextureRect rect{ 0.f, 0.f, 1.f, 1.f };

    for (size_t i = 0; i < particleCount; ++i) {
        if (camera.contains(t1::tile(soa.position[i])))
            renderer.draw(rect, soa.position[i], soa.size[i], soa.size[i] / 2, soa.angle[i], soa.color[i]);
    }
}
