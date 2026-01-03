#pragma once

struct MobSoA;
class Camera;
class ShellManager;

namespace turrets {
    void processTurrets(MobSoA& soa, ShellManager& shells);
    void drawTurrets(const MobSoA& soa, const Camera& camera);
}