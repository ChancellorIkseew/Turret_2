#pragma once
#include "engine/render/texture_rect.hpp"

struct ParticleSoA;
class Assets;
class Camera;
class Renderer;

class ParticlesDrawer {
public:
    void draw(const Camera& camera, Renderer& renderer, const ParticleSoA& soa);
};
