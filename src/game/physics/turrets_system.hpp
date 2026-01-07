#pragma once

struct MobSoA;
class Camera;
class ShellManager;
class SoundQueue;

namespace turrets {
    void processTurrets(MobSoA& soa, ShellManager& shells, SoundQueue& sounds, const Camera& camera);
    void drawTurrets(const MobSoA& soa, const Camera& camera);
}
