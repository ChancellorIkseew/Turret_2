#pragma once
#include <SDL3/SDL_pixels.h>
#include <vector>

struct ParticleSoA;
class Assets;
class Camera;
class Renderer;

class ParticlesDrawer {
    std::vector<float> vertexPositions;
    std::vector<float> vertexUVs;
    std::vector<SDL_FColor> vertexColors;
    std::vector<int> indexCache;
    float u0, v0, u1, v1;
public:
    ParticlesDrawer(const Assets& assets) { updateTexture(assets); }
    void updateTexture(const Assets& assets);
    void draw(const Camera& camera, const Renderer& renderer, const ParticleSoA& soa);
};
