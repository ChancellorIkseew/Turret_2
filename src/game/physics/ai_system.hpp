#pragma once

struct MobSoA;
class PlayerController;

namespace ai {
    void updateMobAI(MobSoA& soa, const PlayerController& playerController);
}
