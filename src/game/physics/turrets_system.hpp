#pragma once

struct MobSoA;
class Camera;
class Renderer;
class ShellManager;
class SoundQueue;

namespace turrets {
    void processTurrets(MobSoA& soa, ShellManager& shells, SoundQueue& sounds, const Camera& camera);
    void drawTurrets(const MobSoA& soa, const Camera& camera, const Renderer& renderer);
}
