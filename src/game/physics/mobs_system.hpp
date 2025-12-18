#pragma once

struct MobSoA;
class Camera;

namespace mobs {
    void processMobs(MobSoA& soa);
    void drawMobs(const MobSoA& soa, const Camera& camera, const float tickOfset);
}