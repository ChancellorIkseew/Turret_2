#pragma once

struct MobSoA;
struct ShellSoA;
class BlockManager;
class Camera;
class ChunkGrid;
class Presets;
class Renderer;
class ShellManager;

namespace shells {
    void processShells(ShellSoA& soa, MobSoA& mobs, const ChunkGrid& chunks, BlockManager& blocks);
    void cleanupShells(ShellManager& manager, const Presets& presets);
    void drawShells(const ShellSoA& soa, const Presets& presets, const Camera& camera, const Renderer& renderer);
}
