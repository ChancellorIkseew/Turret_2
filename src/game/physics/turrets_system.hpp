#pragma once

struct TurretComponents;
class Camera;
class Renderer;
class ParticleManager;
class Presets;
class ShellManager;
class SoundQueue;

namespace turrets {
    void processTurrets(TurretComponents& soa, ShellManager& shells, ParticleManager& particles,
        const Presets& presets, SoundQueue& sounds, const Camera& camera);
    void drawTurrets(TurretComponents&& soa, const Presets& presets, const Camera& camera, const Renderer& renderer);
}
