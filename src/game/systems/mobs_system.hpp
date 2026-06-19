#pragma once
#include <cstdint>

struct MobSoA;
class BlockMap;
class Camera;
class ChunkGrid;
class MobManager;
class PlayerController;
class Presets;
class Renderer;

namespace mobs {
    void processMobs(MobSoA& soa, const ChunkGrid& chunks, const BlockMap& blocks, const Presets& presets);
    void cleanupMobs(MobManager& manager, const Presets& presets, PlayerController& plCtr);
    void drawMobs(MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer, const uint64_t tickCount);
    void drawMobShields(MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer, const uint64_t tickCount);
}
