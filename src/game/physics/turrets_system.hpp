#pragma once

struct MobSoA;
class Camera;
class ShellManager;
class Audio;

namespace turrets {
    void processTurrets(MobSoA& soa, ShellManager& shells, const Camera& camera, Audio& audioManager);
    void drawTurrets(const MobSoA& soa, const Camera& camera);
}