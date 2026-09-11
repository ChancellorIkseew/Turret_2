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
    const auto mob = findPlayerControlled(mobs.shootingData);
    const auto turret = findPlayerControlled(turrets.shootingData);
    const PixelCoord mousePosition = camera.fromScreenToMap(input.getMouseCoord());
    move(input);
    shoot(mousePosition, input);
    mine();
    moveCamera(mobs, mob, paused, camera, input);
    m_unitSelected = m_holdsBlock ? UnitSelected{} : setectUnit(mousePosition, mobs, turrets, mob, turret, presets);
    if (!m_holdsBlock && input.jactive(Control_unit))
        captureUnit(mobs, turrets, mob, turret, presets);
}

UnitSelected PlCtr::setectUnit(const PixelCoord mousePosition, MobSoA& mobs, TurretSoA& turrets,
    const std::optional<size_t> controlledMob, const std::optional<size_t> controlledTurret, const Presets& presets) const {
    for (size_t i = 0; i < mobs.mobCount; ++i) {
        if (mobs.teamID[i] != m_playerTeamID || controlledMob && i == *controlledMob)
            continue;
        const float hitboxRadius = presets.getMob(mobs.preset[i]).hitboxRadius;
        if (t1::areCloserCircle(mobs.position[i], mousePosition, hitboxRadius * 0.9f))
            return UnitSelected{ .mob = i, .turret = std::nullopt };
    }
    for (size_t i = 0; i < turrets.turretCount; ++i) {
        if (turrets.teamID[i] != m_playerTeamID || controlledTurret && i == *controlledTurret)
            continue;
        const PixelCoord size = presets.getTurret(turrets.preset[i]).visual.size;
        const float hitboxRadius = static_cast<float>(std::bit_ceil(static_cast<uint32_t>(size.y / 2.f)));
        if (t1::areCloserRect(turrets.position[i], mousePosition, hitboxRadius * 0.9f))
            return UnitSelected{ .mob = std::nullopt, .turret = i };
    }
    return UnitSelected{};
}

void PlCtr::captureUnit(MobSoA& mobs, TurretSoA& turrets,
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

    if (m_unitSelected.mob) {
        mobs.motionData[*m_unitSelected.mob].aiType = MovingAI::player_controlled;
        mobs.shootingData[*m_unitSelected.mob].aiType = ShootingAI::player_controlled;
    }
    if (m_unitSelected.turret) {
        turrets.shootingData[*m_unitSelected.turret].aiType = ShootingAI::player_controlled;
    }
}
