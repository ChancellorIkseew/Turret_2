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

void PlCtr::shoot(const PixelCoord mousePosition, const Input& input) {
    m_aimCoord = mousePosition;
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
    const Unit unitControlled{ .mob = findPlayerControlled(mobs.shootingData), .turret = findPlayerControlled(turrets.shootingData) };
    const PixelCoord mousePosition = camera.fromScreenToMap(input.getMouseCoord());
    move(input);
    shoot(mousePosition, input);
    mine();
    moveCamera(mobs, unitControlled.mob, paused, camera, input);
    m_unitSelected = m_holdsBlock ? Unit{} : setectUnit(mousePosition, mobs, turrets, unitControlled, presets);
    if (!m_holdsBlock && input.jactive(Control_unit))
        captureUnit(mobs, turrets, unitControlled, presets);
}

PlCtr::Unit PlCtr::setectUnit(const PixelCoord mousePosition, const MobSoA& mobs, const TurretSoA& turrets,
    const Unit controlled, const Presets& presets) const {
    for (size_t i = 0; i < mobs.mobCount; ++i) {
        if (mobs.teamID[i] != m_playerTeamID || controlled.mob && i == *controlled.mob)
            continue;
        const float hitboxRadius = presets.getMob(mobs.preset[i]).hitboxRadius;
        if (t1::areCloserCircle(mobs.position[i], mousePosition, hitboxRadius * 0.9f))
            return Unit{ .mob = i, .turret = std::nullopt };
    }
    for (size_t i = 0; i < turrets.turretCount; ++i) {
        if (turrets.teamID[i] != m_playerTeamID || controlled.turret && i == *controlled.turret)
            continue;
        const PixelCoord size = presets.getTurret(turrets.preset[i]).visual.size;
        const float hitboxRadius = static_cast<float>(std::bit_ceil(static_cast<uint32_t>(size.y / 2.f)));
        if (t1::areCloserRect(turrets.position[i], mousePosition, hitboxRadius * 0.9f))
            return Unit{ .mob = std::nullopt, .turret = i };
    }
    return Unit{};
}

void PlCtr::captureUnit(MobSoA& mobs, TurretSoA& turrets, const Unit controlled, const Presets& presets) const {
    if (controlled.mob) {
        const auto& preset = presets.getMob(mobs.preset[*controlled.mob]);
        mobs.motionData[*controlled.mob].aiType = preset.defaultMovingAI;
        mobs.shootingData[*controlled.mob].aiType = preset.defaultShootingAI;
    }
    if (controlled.turret) {
        // TODO: const auto& preset = presets.getTurret(turrets.preset[*controlled.turret]); // when it would be implemented
        turrets.shootingData[*controlled.turret].aiType = ShootingAI::basic;
    }

    if (m_unitSelected.mob) {
        mobs.motionData[*m_unitSelected.mob].aiType = MovingAI::player_controlled;
        mobs.shootingData[*m_unitSelected.mob].aiType = ShootingAI::player_controlled;
    }
    if (m_unitSelected.turret) {
        turrets.shootingData[*m_unitSelected.turret].aiType = ShootingAI::player_controlled;
    }
}
