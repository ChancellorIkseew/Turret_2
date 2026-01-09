#pragma once

struct MobSoA;
class Camera;
class MobManager;
class Presets;
class Renderer;

namespace mobs {
    void processMobs(MobSoA& soa, const Presets& presets);
    void cleanupMobs(MobManager& manager, const Presets& presets);
    void drawMobs(MobSoA& soa, const Presets& presets, const Camera& camera, const Renderer& renderer);
}
