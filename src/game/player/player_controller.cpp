#include "player_controller.hpp"
//
#include "camera.hpp"
#include "engine/assets/presets.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/window/input/input.hpp"
#include "game/common/team.hpp"

using PlCtr = PlayerController;

void PlCtr::shoot(const Input& input, const Camera& camera) {
    aimCoord = camera.fromScreenToMap(input.getMouseCoord());
    shooting = input.active(Shoot);
}

void PlCtr::mine() {

}

void PlCtr::move(const Input& input, Camera& camera, const MobManager& mobs, const bool isPaused) {
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
    if (state == State::control_camera || isPaused) {
        camera.move(delta);
        camera.moveByMouse(input);
    }
    else /*State::control_mob*/ {
        auto& soa = mobs.getSoa();
        for (size_t i = 0; i < soa.mobCount; ++i) {
            if (soa.motionData[i].aiType == MovingAI::player_controlled) {
                camera.setCenter(soa.position[i]);
                goto ok;
            }  
        }
        state = State::control_camera;
        ok:     
    }
    camera.scale(input);
}

void PlCtr::update(const Input& input, Camera& camera, const bool paused, MobManager& mobs, const Presets& presets) {
    if (input.jactive(Control_unit))
        captureMob(input, camera, mobs, presets);
    move(input, camera, mobs, paused);
    shoot(input, camera);
    mine();
}

void PlCtr::captureMob(const Input& input, const Camera& camera, MobManager& mobs, const Presets& presets) {
    auto& soa = mobs.getSoa();
    for (size_t i = 0; i < soa.mobCount; ++i) {
        auto& movingAI = soa.motionData[i].aiType;
        auto& shootingAI = soa.shootingData[i].aiType;
        if (movingAI == MovingAI::player_controlled || shootingAI == ShootingAI::player_controlled) {
            const auto& preset = presets.getMob(soa.preset[i]);
            movingAI = preset.defaultMovingAI;
            shootingAI = preset.defaultShootingAI;
        }
    }
    state = State::control_camera;

    const PixelCoord mousePosition = input.getMouseCoord();
    for (size_t i = 0; i < soa.mobCount; ++i) {
        if (soa.teamID[i] != playerTeam->getID())
            continue;
        if (t1::areCloserRect(camera.fromMapToScreen(soa.position[i]), mousePosition, 20.f)) {
            soa.motionData[i].aiType = MovingAI::player_controlled;
            soa.shootingData[i].aiType = ShootingAI::player_controlled;
            state = State::control_mob;
        }
    }
}
