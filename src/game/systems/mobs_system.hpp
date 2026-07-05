#pragma once
#include <cstdint>
#include "game/common/physics_base.hpp"

struct MobSoA;
class BlockMap;
class Camera;
class ChunkGrid;
class MobsPool;
class Presets;
class Renderer;

namespace mobs {
    void processMobs(MobSoA& soa, const ChunkGrid& chunks, const BlockMap& blocks, const Presets& presets);
    void cleanupMobs(MobsPool& mobsPool, const Presets& presets);
    //
    void drawHealthBars(const MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer);
    void drawMobs(MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer, const uint64_t tickCount);
    void drawMobShields(const MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer, const uint64_t tickCount);
    void drawEnemyMarkers(const TeamID playerTeamID, const MobSoA& soa, const Camera& camera, Renderer& renderer);
}
