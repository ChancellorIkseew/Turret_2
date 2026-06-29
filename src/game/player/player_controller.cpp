#include "player_controller.hpp"
//
#include "camera.hpp"
#include "engine/assets/presets.hpp"
#include "engine/window/input/input.hpp"
#include "game/entities/mob_manager.hpp"
#include "game/entities/turret_manager.hpp"

using PlCtr = PlayerController;

static std::optional<size_t> findPlayerControlled(const std::vector<ShootingData>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i].aiType == ShootingAI::player_controlled)
            return i;
    }
    return std::nullopt;
}

void PlCtr::shoot(const Input& input, const Camera& camera) {
    aimCoord = camera.fromScreenToMap(input.getMouseCoord());
    shooting = !holdsBlock && input.active(Build_Shoot);
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
    motionVector = delta;
}

void PlCtr::moveCamera(const MobSoA& mobs, const std::optional<size_t> mob, const bool paused, Camera& camera, const Input& input) const {
    if (!mob || paused) {
        camera.move(motionVector);
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
        captureMobOrTurret(input, camera, mobs, turrets, mob, turret, presets);
}

void PlCtr::captureMobOrTurret(const Input& input, const Camera& camera, MobSoA& mobs, TurretSoA& turrets,
    const std::optional<size_t> mob, const std::optional<size_t> turret, const Presets& presets) const {
    if (mob) {
        const auto& preset = presets.getMob(mobs.preset[*mob]);
        mobs.motionData[*mob].aiType = preset.defaultMovingAI;
        mobs.shootingData[*mob].aiType = preset.defaultShootingAI;
    }
    if (turret) {
        // TODO: const auto& preset = presets.getTurret(mobs.preset[*turret]); // when it would be implemented
        turrets.shootingData[*turret].aiType = ShootingAI::basic;
    }

    const PixelCoord mousePosition = input.getMouseCoord();
    for (size_t i = 0; i < mobs.mobCount; ++i) {
        if (mobs.teamID[i] != playerTeamID)
            continue;
        if (t1::areCloserRect(camera.fromMapToScreen(mobs.position[i]), mousePosition, 20.f)) {
            mobs.motionData[i].aiType = MovingAI::player_controlled;
            mobs.shootingData[i].aiType = ShootingAI::player_controlled;
            return; // avoid capcture both mob and turret
        }
    }

    for (size_t i = 0; i < turrets.turretCount; ++i) {
        if (turrets.teamID[i] != playerTeamID)
            continue;
        if (t1::areCloserRect(camera.fromMapToScreen(turrets.position[i]), mousePosition, 20.f)) {
            turrets.shootingData[i].aiType = ShootingAI::player_controlled;
            return; // avoid capcture both mob and turret
        }
    }
}
