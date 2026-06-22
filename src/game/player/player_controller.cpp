#include "player_controller.hpp"
//
#include "camera.hpp"
#include "engine/assets/presets.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/window/input/input.hpp"
#include "game/common/team.hpp"
#include "game/entities/mob_manager.hpp"
#include "game/entities/turret_manager.hpp"

using PlCtr = PlayerController;

void PlCtr::shoot(const Input& input, const Camera& camera) {
    aimCoord = camera.fromScreenToMap(input.getMouseCoord());
    shooting = input.active(Shoot);
}

void PlCtr::mine() {

}

void PlCtr::move(const Input& input, Camera& camera, const MobSoA& mobs, const bool isPaused) {
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
    if (state != State::control_mob || isPaused) {
        camera.move(delta);
        camera.moveByMouse(input);
    }
    else /*State::control_mob*/ {
        for (size_t i = 0; i < mobs.mobCount; ++i) {
            if (mobs.motionData[i].aiType == MovingAI::player_controlled) {
                camera.setCenter(mobs.position[i]);
                goto ok;
            }  
        }
        state = State::control_camera;
        ok:     
    }
    camera.scale(input);
}

void PlCtr::update(const Input& input, Camera& camera, const bool paused, MobSoA& mobs, TurretSoA& turrets, const Presets& presets) {
    if (input.jactive(Control_unit))
        captureMob(input, camera, mobs, turrets, presets);
    move(input, camera, mobs, paused);
    shoot(input, camera);
    mine();
}

void PlCtr::captureMob(const Input& input, const Camera& camera, MobSoA& mobs, TurretSoA& turrets, const Presets& presets) {
    for (size_t i = 0; i < mobs.mobCount; ++i) {
        auto& movingAI = mobs.motionData[i].aiType;
        auto& shootingAI = mobs.shootingData[i].aiType;
        if (movingAI == MovingAI::player_controlled || shootingAI == ShootingAI::player_controlled) {
            const auto& preset = presets.getMob(mobs.preset[i]);
            movingAI = preset.defaultMovingAI;
            shootingAI = preset.defaultShootingAI;
        }
    }

    for (size_t i = 0; i < turrets.turretCount; ++i) {
        auto& shootingAI = turrets.shootingData[i].aiType;
        if (shootingAI == ShootingAI::player_controlled) {
            //const auto& preset = presets.getTurret(turrets.preset[i]);
            shootingAI = ShootingAI::basic;
        }
    }

    state = State::control_camera;

    const PixelCoord mousePosition = input.getMouseCoord();
    for (size_t i = 0; i < mobs.mobCount; ++i) {
        if (mobs.teamID[i] != playerTeamID)
            continue;
        if (t1::areCloserRect(camera.fromMapToScreen(mobs.position[i]), mousePosition, 20.f)) {
            mobs.motionData[i].aiType = MovingAI::player_controlled;
            mobs.shootingData[i].aiType = ShootingAI::player_controlled;
            state = State::control_mob;
            break;
        }
    }

    for (size_t i = 0; i < turrets.turretCount; ++i) {
        //if (turret.teamID[i] != playerTeam->getID())
            //continue;
        if (t1::areCloserRect(camera.fromMapToScreen(turrets.position[i]), mousePosition, 20.f)) {
            turrets.shootingData[i].aiType = ShootingAI::player_controlled;
            state = State::control_turret;
            break;
        }
    }
}
