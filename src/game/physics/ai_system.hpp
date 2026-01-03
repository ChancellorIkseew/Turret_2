#pragma once

struct MobSoA;
class PlayerController;

namespace ai {
    void updateMovingAI(MobSoA& soa, const PlayerController& playerController);
    void updateShootingAI(MobSoA& soa, const PlayerController& playerController);
}
