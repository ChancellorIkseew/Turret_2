#pragma once

struct MobSoA;
struct TurretComponents;
class BlockMap;
class PlayerController;
class Presets;

namespace ai {
    void updateMovingAI(MobSoA& soa, const Presets& presets, const PlayerController& playerController);
    void updateShootingAI(TurretComponents& soa, const MobSoA& mobs, const BlockMap& blocks,
        const Presets& presets, const PlayerController& playerController);
}
