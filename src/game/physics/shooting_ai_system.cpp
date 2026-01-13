#include "ai_system.hpp"
//
#include <cmath>
#include "engine/assets/presets.hpp"
#include "engine/coords/math.hpp"
#include "game/player/player_controller.hpp"
#include "mob_manager.hpp"

constexpr PixelCoord NO_MOTION(0.0f, 0.0f);

static t1_finline float normalize(const float angle) {
    if (angle >  t1::PI_F) return angle - 2.0f * t1::PI_F;
    if (angle < -t1::PI_F) return angle + 2.0f * t1::PI_F;
    return angle;
}

static t1_finline void rotateTurret(MobSoA& soa, const Presets& presets, const size_t index, const PixelCoord aim) {
    const AngleRad requiredAngle = t1::atan(aim - soa.position[index]);
    const AngleRad deltaAngle = normalize(requiredAngle - soa.turretAngle[index]);
    const AngleRad maxStep = presets.getTurret(presets.getMob(soa.preset[index]).turret).rotationSpeed;
    //
    if (std::abs(deltaAngle) <= maxStep)
        soa.turretAngle[index] = requiredAngle;
    else
        soa.turretAngle[index] += (deltaAngle > 0 ? maxStep : -maxStep);
}

static inline void updatePlayerControlled(MobSoA& soa, const Presets& presets,
    const size_t index, const PlayerController& playerController) {
    soa.shootingData[index].isShooting = playerController.shootingActive();
    rotateTurret(soa, presets, index, playerController.getAimCoord());
}

static inline void updateBasic(MobSoA& soa, const Presets& presets, const size_t index) {
    const float range = presets.getTurret(presets.getMob(soa.preset[index]).turret).range;
    const PixelCoord position = soa.position[index];
    const TeamID teamID = soa.teamID[index];
    soa.shootingData[index].isShooting = false;

    for (size_t i = 0; i < soa.mobCount; ++i) {
        if (teamID == soa.teamID[i])
            continue;
        if (!t1::areCloser(position, soa.position[i], range))
            continue;
        const PixelCoord delta = position - soa.position[i];
        if (t1::pow2f(range) >= t1::pow2f(delta.x) + t1::pow2f(delta.y)) {
            soa.shootingData[index].isShooting = true;
            rotateTurret(soa, presets, index, soa.position[i]);
            return;
        }
    }
}

void ai::updateShootingAI(MobSoA& soa, const Presets& presets, const PlayerController& playerController) {
    const size_t mobCount = soa.id.size();
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
