#include "particles_drawer.hpp"
//
#include "engine/assets/assets.hpp"
#include "engine/render/renderer.hpp"
#include "game/particles/particle_manager.hpp"
#include "game/player/camera.hpp"

constexpr float PARTICLE_HALF_SIZE = 1.0f;

void ParticlesDrawer::updateTexture(const Assets& assets) {
    const SDL_FRect& pixelRect = assets.getAtlas().at("particle_base").rect;
    const PixelCoord atlasSize = assets.getAtlas().getSize();
    u0 = pixelRect.x / atlasSize.x;
    v0 = pixelRect.y / atlasSize.y;
    u1 = (pixelRect.x + pixelRect.w) / atlasSize.x;
    v1 = (pixelRect.y + pixelRect.h) / atlasSize.y;
}

void ParticlesDrawer::draw(const Camera& camera, const Renderer& renderer, const ParticleSoA& soa) {
    const size_t totalParticleCount = soa.particleCount;
    if (totalParticleCount == 0)
        return;
    vertexPositions.clear();
    vertexUVs.clear();
    vertexColors.clear();
    indexCache.clear();

    const PixelCoord translation = camera.getTranslation();
    int currentVertexOffset = 0;

    for (size_t i = 0; i < totalParticleCount; ++i) {
        const PixelCoord worldPosition = soa.position[i];
        if (!camera.contains(t1::tile(worldPosition)))
            continue;

        const PixelCoord screenPosition = worldPosition - translation;

        const uint32_t c = soa.color[i];
        const SDL_FColor vColor = {
            ((c >> 24) & 0xFF) / 255.0f, // R
            ((c >> 16) & 0xFF) / 255.0f, // G
            ((c >> 8) & 0xFF) / 255.0f, // B
            (c & 0xFF) / 255.0f  // A
        };

        // rotation offsets
        const PixelCoord rightOffset = soa.facing[i] * PARTICLE_HALF_SIZE;
        const PixelCoord upOffset(-rightOffset.y, rightOffset.x);

        const PixelCoord vertex0 = screenPosition - rightOffset + upOffset;
        const PixelCoord vertex1 = screenPosition + rightOffset + upOffset;
        const PixelCoord vertex2 = screenPosition + rightOffset - upOffset;;
        const PixelCoord vertex3 = screenPosition - rightOffset - upOffset;;

        vertexPositions.insert(vertexPositions.end(), {
            vertex0.x, vertex0.y, // TL
            vertex1.x, vertex1.y, // TR
            vertex2.x, vertex2.y, // BR
            vertex3.x, vertex3.y  // BL
            });

        vertexUVs.insert(vertexUVs.end(), {
            u0, v0,
            u1, v0,
            u1, v1,
            u0, v1
            });

        for (int j = 0; j < 4; ++j) vertexColors.push_back(vColor);

        indexCache.insert(indexCache.end(), {
            currentVertexOffset + 0, currentVertexOffset + 1, currentVertexOffset + 2,
            currentVertexOffset + 2, currentVertexOffset + 3, currentVertexOffset + 0
            });

        currentVertexOffset += 4;
    }

    if (indexCache.empty())
        return;
    renderer.drawBatchedColored(vertexPositions.data(), vertexUVs.data(), indexCache.data(), vertexColors.data(),
        currentVertexOffset, static_cast<int>(indexCache.size()));
}
