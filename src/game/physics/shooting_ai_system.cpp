#include "ai_system.hpp"
//
#include "engine/coords/math.hpp"
#include "game/player/player_controller.hpp"
#include "mob_manager.hpp"

constexpr PixelCoord NO_MOTION(0.0f, 0.0f);

static inline float atan2f(const PixelCoord p) noexcept { return atan2f(p.x, p.y); }

static t1_finline float normalize(const float angle) {
    if (angle >  t1::PI_F) return angle - 2.0f * t1::PI_F;
    if (angle < -t1::PI_F) return angle + 2.0f * t1::PI_F;
    return angle;
}

static inline void updatePlayerControlled(MobSoA& soa, const size_t index, const PlayerController& playerController) {
    soa.shootingData[index].isShooting = playerController.shootingActive();
    const PixelCoord aim = playerController.getAimCoord();
    const AngleRad requiredAngle = atan2f(aim - soa.position[index]);
    const AngleRad deltaAngle = normalize(requiredAngle - soa.turretAngle[index]);
    const AngleRad maxStep = soa.preset[index]->turret->rotationSpeed;

    if (std::abs(deltaAngle) <= maxStep)
        soa.turretAngle[index] = requiredAngle;
    else
        soa.turretAngle[index] += (deltaAngle > 0 ? maxStep : -maxStep);
}

void ai::updateShootingAI(MobSoA& soa, const PlayerController& playerController) {
    const size_t mobCount = soa.id.size();
    for (size_t i = 0; i < mobCount; ++i) {
        switch (soa.shootingData[i].aiType) {
        case ShootingAI::player_controlled:
            updatePlayerControlled(soa, i, playerController);
            break;
        case ShootingAI::basic:
            //updateBasic(soa, i);
            break;
        }
    }
}