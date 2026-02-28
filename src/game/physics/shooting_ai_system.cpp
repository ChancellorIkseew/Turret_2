#include "ai_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/coords/math.hpp"
#include "game/player/player_controller.hpp"
#include "mob_manager.hpp"
#include "turret_components.hpp"

static inline void updatePlayerControlled(TurretComponents& soa, const Presets& presets,
    const size_t index, const PlayerController& playerController) {
    soa.shootingData[index].isShooting = playerController.shootingActive();
    soa.shootingData[index].target = playerController.getAimCoord();
}

static inline void updateBasic(TurretComponents& soa, const MobSoA& mobs, const Presets& presets, const size_t index) {
    const float range = presets.getTurret(soa.preset[index]).range;
    const PixelCoord position = soa.position[index];
    const TeamID teamID = soa.teamID[index];
    soa.shootingData[index].isShooting = false;

    for (size_t i = 0; i < mobs.mobCount; ++i) {
        if (teamID == mobs.teamID[i])
            continue;
        if (t1::areCloserCircle(position, mobs.position[i], range)) {
            soa.shootingData[index].isShooting = true;
            soa.shootingData[index].target = mobs.position[i];
            return;
        }
    }
}

void ai::updateShootingAI(TurretComponents& soa, const MobSoA& mobs, const Presets& presets,
    const PlayerController& playerController) {
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        switch (soa.shootingData[i].aiType) {
        case ShootingAI::player_controlled:
            updatePlayerControlled(soa, presets, i, playerController);
            break;
        case ShootingAI::basic:
            updateBasic(soa, mobs, presets, i);
            break;
        }
    }
}
