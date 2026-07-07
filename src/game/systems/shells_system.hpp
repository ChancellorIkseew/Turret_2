#pragma once

struct MobSoA;
struct ShellSoA;
class BlockMap;
class Camera;
class ChunkGrid;
class Presets;
class Renderer;
class SoundQueue;
class ShellsPool;
class World;

namespace shells {
    void processShells(World& world, const Presets& presets, SoundQueue& sounds, const Camera& camera);
    void cleanupShells(ShellsPool& shellsPool, const Presets& presets);
    void drawShells(const ShellSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer);
    void drawShellsLighting(const ShellSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer);
}
