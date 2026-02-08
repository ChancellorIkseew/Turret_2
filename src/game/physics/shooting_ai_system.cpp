#include "ai_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/coords/math.hpp"
#include "game/player/player_controller.hpp"
#include "mob_manager.hpp"

static inline void updatePlayerControlled(MobSoA& soa, const Presets& presets,
    const size_t index, const PlayerController& playerController) {
    soa.shootingData[index].isShooting = playerController.shootingActive();
    soa.shootingData[index].target = playerController.getAimCoord();
}

static inline void updateBasic(MobSoA& soa, const Presets& presets, const size_t index) {
    const float range = presets.getTurret(presets.getMob(soa.preset[index]).turret).range;
    const PixelCoord position = soa.position[index];
    const TeamID teamID = soa.teamID[index];
    soa.shootingData[index].isShooting = false;

    for (size_t i = 0; i < soa.mobCount; ++i) {
        if (teamID == soa.teamID[i])
            continue;
        if (t1::areCloserCircle(position, soa.position[i], range)) {
            soa.shootingData[index].isShooting = true;
            soa.shootingData[index].target = soa.position[i];
            return;
        }
    }
}

void ai::updateShootingAI(MobSoA& soa, const Presets& presets, const PlayerController& playerController) {
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        switch (soa.shootingData[i].aiType) {
        case ShootingAI::player_controlled:
            updatePlayerControlled(soa, presets, i, playerController);
            break;
        case ShootingAI::basic:
            updateBasic(soa, presets, i);
            break;
        }
    }
}
