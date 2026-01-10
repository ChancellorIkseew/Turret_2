#pragma once

struct MobSoA;
class PlayerController;
class Presets;

namespace ai {
    void updateMovingAI(MobSoA& soa, const Presets& presets, const PlayerController& playerController);
    void updateShootingAI(MobSoA& soa, const Presets& presets, const PlayerController& playerController);
}
