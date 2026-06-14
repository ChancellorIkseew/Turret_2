#include "particles_drawer.hpp"
//
#include "engine/assets/assets.hpp"
#include "engine/render/renderer.hpp"
#include "game/particles/particle_manager.hpp"
#include "game/player/camera.hpp"

constexpr PixelCoord PARTICLE_SIZE(2.f, 2.f);
constexpr PixelCoord PARTICLE_ORIGIN(1.f, 1.f);

void ParticlesDrawer::updateTexture(const Assets& assets) {
    particleBase = assets.getAtlas().at("particle_base");
}

void ParticlesDrawer::draw(const Camera& camera, Renderer& renderer, const ParticleSoA& soa) {
    const size_t totalParticleCount = soa.particleCount;
    if (totalParticleCount == 0)
        return;
    for (size_t i = 0; i < totalParticleCount; ++i) {
        renderer.draw(particleBase, soa.position[i], PARTICLE_SIZE, PARTICLE_ORIGIN, 0.f, soa.color[i]);
    }
}
