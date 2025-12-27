#pragma once

struct MobSoA;
class Camera;
class MobManager;

namespace mobs {
    void processMobs(MobSoA& soa);
    void cleanupMobs(MobManager& manager);
    void drawMobs(const MobSoA& soa, const Camera& camera, const float tickOfset);
}
