#pragma once

struct MobSoA;
struct TurretComponents;
class BlockManager;
class PlayerController;
class Presets;

namespace ai {
    void updateMovingAI(MobSoA& soa, const Presets& presets, const PlayerController& playerController);
    void updateShootingAI(TurretComponents& soa, const MobSoA& mobs, const BlockManager& blocks,
        const Presets& presets, const PlayerController& playerController);
}
