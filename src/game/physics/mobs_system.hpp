#pragma once

struct MobSoA;
class Camera;
class MobManager;
class Renderer;

namespace mobs {
    void processMobs(MobSoA& soa);
    void cleanupMobs(MobManager& manager);
    void drawMobs(MobSoA& soa, const Camera& camera, const Renderer& renderer);
}
