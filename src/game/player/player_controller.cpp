#include "player_controller.hpp"
//
#include "camera.hpp"
#include "engine/assets/presets.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/window/input/input.hpp"
#include "game/physics/team/team.hpp"

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
        const size_t index = mobs.getSoaIndexByMobID(targetMobID);
        camera.setPosition(mobs.getSoa().position[index]);
    }
    camera.scale(input);
}

void PlCtr::update(const Input& input, Camera& camera, const bool paused, MobManager& mobs, const Presets& presets) {
    move(input, camera, mobs, paused);
    shoot(input, camera);
    mine();
    captureMob(input, camera, mobs, presets);
}

void PlCtr::captureMob(const Input& input, const Camera& camera, MobManager& mobs, const Presets& presets) {
    if (!input.jactive(Control_unit))
        return;
    const auto& soa = mobs.getSoa();
    const size_t mobCount = soa.position.size();
    for (size_t i = 0; i < mobCount; ++i) {
        if (soa.teamID[i] != playerTeam->getID())
            continue;
        if (t1::areCloser(camera.fromMapToScreen(soa.position[i]), input.getMouseCoord(), 20.f))
            return setTarget(mobs, soa.id[i], presets);     
    }
    resetTarget(mobs, presets);
}

void PlCtr::setTarget(MobManager& mobs, const MobID mobID, const Presets& presets) {
    resetTarget(mobs, presets); // Reset current target before set new.
    if (mobID == IDManager<MobID>::INVALID_ID)
        return;
    const size_t index = mobs.getSoaIndexByMobID(mobID);
    mobs.getSoa().motionData[index].aiType = MovingAI::player_controlled;
    mobs.getSoa().shootingData[index].aiType = ShootingAI::player_controlled;
    targetMobID = mobID;
    state = State::control_mob;
}

void PlCtr::resetTarget(MobManager& mobs, const Presets& presets) {
    if (targetMobID == IDManager<MobID>::INVALID_ID)
        return;
    const size_t index = mobs.getSoaIndexByMobID(targetMobID);
    mobs.getSoa().motionData[index].aiType =   presets.getMob(mobs.getSoa().preset[index]).defaultMovingAI;
    mobs.getSoa().shootingData[index].aiType = presets.getMob(mobs.getSoa().preset[index]).defaultShootingAI;
    targetMobID = IDManager<MobID>::INVALID_ID;
    state = State::control_camera;
}
