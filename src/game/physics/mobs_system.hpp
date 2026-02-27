#pragma once
#include <cstdint>

struct MobSoA;
class BlockManager;
class Camera;
class ChunkGrid;
class MobManager;
class PlayerController;
class Presets;
class Renderer;

namespace mobs {
    void processMobs(MobSoA& soa, const ChunkGrid& chunks, const BlockManager& blocks);
    void cleanupMobs(MobManager& manager, const Presets& presets, PlayerController& plCtr);
    void drawMobs(MobSoA& soa, const Presets& presets, const Camera& camera, const Renderer& renderer, const uint64_t tickCount);
}
