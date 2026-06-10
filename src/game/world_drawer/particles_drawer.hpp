#pragma once
#include <SDL3/SDL_pixels.h>
#include <vector>
#include "engine/render/texture_rect.hpp"

struct ParticleSoA;
class Assets;
class Camera;
class Renderer;

class ParticlesDrawer {
    TextureRect particleBase = NULL_TEXTURE_RECT;
public:
    ParticlesDrawer(const Assets& assets) { updateTexture(assets); }
    void updateTexture(const Assets& assets);
    void draw(const Camera& camera, Renderer& renderer, const ParticleSoA& soa);
};
