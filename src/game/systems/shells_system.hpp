#pragma once

struct MobSoA;
struct ShellSoA;
class BlockMap;
class Camera;
class ChunkGrid;
class Presets;
class Renderer;
class ShellsPool;

namespace shells {
    void processShells(ShellSoA& soa, MobSoA& mobs, const ChunkGrid& chunks, BlockMap& blocks);
    void cleanupShells(ShellsPool& shellsPool, const Presets& presets);
    void drawShells(const ShellSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer);
    void drawShellsLighting(const ShellSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer);
}
