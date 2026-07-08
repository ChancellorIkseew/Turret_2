#pragma once
#include <cstdint>

struct TurretComponents;
class Camera;
class Renderer;
class ParticlesPool;
class Presets;
class ShellsPool;
class SoundQueue;

namespace turrets {
    void processTurrets(TurretComponents& soa, ShellsPool& shells, ParticlesPool& particles,
        const Presets& presets, SoundQueue& sounds, const Camera& camera, const uint64_t timeMs);
    void drawTurrets(TurretComponents&& soa, const Presets& presets, const Camera& camera, Renderer& renderer);
    void drawShadows(TurretComponents&& soa, const Presets& presets, const Camera& camera, Renderer& renderer);
}
