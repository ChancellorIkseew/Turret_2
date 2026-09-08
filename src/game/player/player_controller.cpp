#include "player_controller.hpp"
//
#include <bit>
#include "camera.hpp"
#include "engine/assets/presets.hpp"
#include "engine/window/input/input.hpp"
#include "game/entities/mobs_pool.hpp"
#include "game/entities/turrets_pool.hpp"

using PlCtr = PlayerController;

static std::optional<size_t> findPlayerControlled(const std::vector<ShootingData>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i].aiType == ShootingAI::player_controlled)
            return i;
    }
    return std::nullopt;
}

void PlCtr::shoot(const Input& input, const Camera& camera) {
    m_aimCoord = camera.fromScreenToMap(input.getMouseCoord());
    m_shooting = !m_holdsBlock && input.active(Build_Shoot);
}

void PlCtr::mine() {

}

void PlCtr::move(const Input& input) {
    PixelCoord delta(0.0f, 0.0f);
    if (input.active(Move_up))
        delta.y -= 1.0f;
    if (input.active(Move_left))
        delta.x -= 1.0f;
    if (input.active(Move_down))
        delta.y += 1.0f;
    if (input.active(Move_right))
        delta.x += 1.0f;
    m_motionVector = delta;
}

void PlCtr::moveCamera(const MobSoA& mobs, const std::optional<size_t> mob, const bool paused, Camera& camera, const Input& input) const {
    if (!mob || paused) {
        camera.move(m_motionVector);
        camera.moveByMouse(input);
    }  
    else
        camera.setTargetCenter(mobs.position[*mob]);
    camera.scale(input);
}

void PlCtr::update(const Input& input, Camera& camera, const bool paused, MobSoA& mobs, TurretSoA& turrets, const Presets& presets) {
    const auto mob = findPlayerControlled(mobs.shootingData);
    const auto turret = findPlayerControlled(turrets.shootingData);
    move(input);
    shoot(input, camera);
    mine();
    moveCamera(mobs, mob, paused, camera, input);
    if (input.jactive(Control_unit))
        captureMobOrTurret(camera.fromScreenToMap(input.getMouseCoord()), mobs, turrets, mob, turret, presets);
}

void PlCtr::captureMobOrTurret(const PixelCoord mousePosition, MobSoA& mobs, TurretSoA& turrets,
    const std::optional<size_t> controlledMob, const std::optional<size_t> controlledTurret, const Presets& presets) const {
    if (controlledMob) {
        const auto& preset = presets.getMob(mobs.preset[*controlledMob]);
        mobs.motionData[*controlledMob].aiType = preset.defaultMovingAI;
        mobs.shootingData[*controlledMob].aiType = preset.defaultShootingAI;
    }
    if (controlledTurret) {
        // TODO: const auto& preset = presets.getTurret(turrets.preset[*turret]); // when it would be implemented
        turrets.shootingData[*controlledTurret].aiType = ShootingAI::basic;
    }

    for (size_t i = 0; i < mobs.mobCount; ++i) {
        if (mobs.teamID[i] != m_playerTeamID)
            continue;
        const float hitboxRadius = presets.getMob(mobs.preset[i]).hitboxRadius;

        if (t1::areCloserCircle(mobs.position[i], mousePosition, hitboxRadius * 0.9f)) {
            mobs.motionData[i].aiType = MovingAI::player_controlled;
            mobs.shootingData[i].aiType = ShootingAI::player_controlled;
            return; // avoid capcture both mob and turret
        }
    }

    for (size_t i = 0; i < turrets.turretCount; ++i) {
        if (turrets.teamID[i] != m_playerTeamID)
            continue;
        const PixelCoord size = presets.getTurret(turrets.preset[i]).visual.size;
        const float hitboxRadius = static_cast<float>(std::bit_ceil(static_cast<uint32_t>(size.y / 2.f)));

        if (t1::areCloserRect(turrets.position[i], mousePosition, hitboxRadius * 0.9f)) {
            turrets.shootingData[i].aiType = ShootingAI::player_controlled;
            return; // avoid capcture both mob and turret
        }
    }
}
