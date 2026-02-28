#pragma once

struct MobSoA;
struct TurretComponents;
class PlayerController;
class Presets;

namespace ai {
    void updateMovingAI(MobSoA& soa, const Presets& presets, const PlayerController& playerController);
    void updateShootingAI(TurretComponents& soa, const MobSoA& mobs, const Presets& presets,
        const PlayerController& playerController);
}
