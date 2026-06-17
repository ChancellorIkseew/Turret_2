#pragma once
#include "engine/render/renderer.hpp"
#include "game/particles/particle_manager.hpp"
#include "game/player/camera.hpp"

constexpr PixelCoord PARTICLE_SIZE(15.f, 15.f);
constexpr PixelCoord PARTICLE_ORIGIN(7.5f, 7.5f);

void drawParticles(const Camera& camera, Renderer& renderer, const ParticleSoA& soa) {
    const size_t totalParticleCount = soa.particleCount;
    if (totalParticleCount == 0)
        return;
    constexpr TextureRect rect{ 0.f, 0.f, 1.f, 1.f };
    const float scale = camera.getMapScale();
    const PixelCoord translation = camera.getTranslation();
    const PixelCoord scaledSize = PARTICLE_SIZE * scale;
    const PixelCoord scaledOrigin = PARTICLE_ORIGIN * scale;

    for (size_t i = 0; i < totalParticleCount; ++i) {
        const PixelCoord screenPos = (soa.position[i] - translation) * scale;
        renderer.draw(rect, screenPos, scaledSize, scaledOrigin, 0.f, soa.color[i]);
    }
}
