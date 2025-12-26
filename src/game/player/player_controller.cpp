#include "player_controller.hpp"
//
#include "camera.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/window/input/input.hpp"
#include "game/physics/team/team.hpp"

void PlayerController::shoot(const Input& input, const Camera& camera) {
    const auto mouseCoord = camera.fromScreenToMap(input.getMouseCoord());
    aimCoord.store(mouseCoord, std::memory_order_relaxed);

    const bool flag = input.active(Shoot);
    shooting.store(flag, std::memory_order_relaxed);
}

void PlayerController::mine() {

}

void PlayerController::move(const Input& input, Camera& camera, const MobManager& mobs, const float tickOfset) {
    PixelCoord delta(0.0f, 0.0f);

    if (input.active(Move_up))
        delta.y -= 1.0f;
    if (input.active(Move_left))
        delta.x -= 1.0f;
    if (input.active(Move_down))
        delta.y += 1.0f;
    if (input.active(Move_right))
        delta.x += 1.0f;

    motionVector.store(delta, std::memory_order_relaxed);
    if (state == State::control_camera) {
        camera.move(delta);
        camera.moveByMouse(input);
    }
    else if (state == State::control_mob) {
        const size_t index = mobs.getSoaIndexByMobID(targetMobID);
        camera.setPosition(mobs.getSoa().position[index] + mobs.getSoa().velocity[index] * tickOfset);
    }
    camera.scale(input);
}

void PlayerController::update(Engine& engine, MobManager& mobs, const float tickOfset) {
    const Input& input = engine.getMainWindow().getInput();
    Camera& camera = engine.getCamera();
    const GUI& gui = engine.getGUI();

    if (gui.hasOverlaped() || !gui.isMouseFree())
        return;
    move(engine.getMainWindow().getInput(), camera, mobs, tickOfset);
    shoot(input, camera);
    mine();
    captureMob(input, camera, mobs);
}

void PlayerController::captureMob(const Input& input, const Camera& camera, MobManager& mobs) {
    if (state == State::no_control || !input.jactive(Control_unit))
        return;
    const auto& soa = mobs.getSoa();
    const size_t mobCount = soa.position.size();
    for (size_t i = 0; i < mobCount; ++i) {
        if (soa.teamID[i] != playerTeam->getID())
            continue;
        if (t1::areCloser(camera.fromMapToScreen(soa.position[i]), input.getMouseCoord(), 20.f))
            return setTarget(mobs, soa.id[i]);     
    }
    resetTarget(mobs);
}

void PlayerController::setTarget(MobManager& mobs, const MobID mobID) {
    resetTarget(mobs); // Reset current target before set new.
    if (mobID == IDManager<MobID>::INVALID_ID)
        return;
    const size_t index = mobs.getSoaIndexByMobID(mobID);
    mobs.getSoa().motionData[index].aiType = MovingAI::player_controlled;
    mobs.getSoa().shootingData[index].aiType = ShootingAI::player_controlled;
    targetMobID = mobID;
    state = State::control_mob;
}

void PlayerController::resetTarget(MobManager& mobs) {
    if (targetMobID == IDManager<MobID>::INVALID_ID)
        return;
    const size_t index = mobs.getSoaIndexByMobID(targetMobID);
    mobs.getSoa().motionData[index].aiType = mobs.getSoa().preset[index]->defaultMovingAI;
    mobs.getSoa().shootingData[index].aiType = mobs.getSoa().preset[index]->defaultShootingAI;
    targetMobID = IDManager<MobID>::INVALID_ID;
    state = State::control_camera;
}
