#pragma once

struct MobSoA;
class Camera;
class Renderer;
class Presets;
class ShellManager;
class SoundQueue;

namespace turrets {
    void processTurrets(MobSoA& soa, ShellManager& shells, const Presets& presets, SoundQueue& sounds, const Camera& camera);
    void drawTurrets(const MobSoA& soa, const Presets& presets, const Camera& camera, const Renderer& renderer);
}
