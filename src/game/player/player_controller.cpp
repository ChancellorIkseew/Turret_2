#include "player_controller.hpp"
//
#include <cmath>
#include "camera.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/gui/gui.hpp"
#include "engine/window/input/input.hpp"
#include "game/physics/team/team.hpp"
#include "game/physics/mob_ai.hpp"

void PlayerController::shoot(const Camera& camera) {
	const auto mouseCoord = camera.fromScreenToMap(Input::getMouseCoord());
	aimCoord.store(mouseCoord, std::memory_order_relaxed);

	const bool flag = Input::active(Shoot);
	shooting.store(flag, std::memory_order_relaxed);
}

void PlayerController::mine() {

}

void PlayerController::move(Camera& camera, const float tickOfset) {
	if (guiActive)
		return;
	PixelCoord delta(0.0f, 0.0f);

	if (Input::active(Move_up))
		delta.y -= 1.0f;
	if (Input::active(Move_left))
		delta.x -= 1.0f;
	if (Input::active(Move_down))
		delta.y += 1.0f;
	if (Input::active(Move_right))
		delta.x += 1.0f;

	motionVector.store(delta, std::memory_order_relaxed);
	if (state == State::control_camera) {
		camera.move(delta);
		camera.moveByMouse();
	}
    else if (state == State::control_mob) {
		camera.setPosition(targetedMob->position + targetedMob->velocity * tickOfset);
	}
	camera.scale();
}

void PlayerController::update(const Team& playerTeam, Camera& camera, const GUI& gui, const float tickOfset) {
	guiActive = gui.hasOverlaped() || !gui.isMouseFree();
	move(camera, tickOfset);
	shoot(camera);
	mine();
	captureMob(playerTeam, camera);
}

void PlayerController::captureMob(const Team& playerTeam, const Camera& camera) {
	if (!Input::jactive(Control_unit))
		return;
	for (const auto& mob : playerTeam.getMobs()) {
		if (t1::areCloser(camera.fromMapToScreen(mob.position), Input::getMouseCoord(), 20.f))
			return setTarget(mob);
	}
	resetTarget();
}

void PlayerController::setTarget(const Mob& mob) {
	resetTarget();
	targetedMob = const_cast<Mob*>(&mob);
	targetedMob->movingAI = std::make_unique<PlayerControlledMoving>();
	//TODO: targetedMob->shootingAI = std::make_unique<PlayerControlledShooting>();
	state = State::control_mob;
}

void PlayerController::resetTarget() {
	if (!targetedMob)
		return;
	targetedMob->movingAI = std::make_unique<BasicMovingAI>();
	//TODO: targetedMob->shootingAI = std::make_unique<BasicShootingAI>();
	targetedMob = nullptr;
	state = State::control_camera;
}
