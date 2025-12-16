#include "player_controller.hpp"
//
#include <cmath>
#include "camera.hpp"
#include "engine/engine.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/gui/gui.hpp"
#include "engine/window/input/input.hpp"
#include "game/physics/team/team.hpp"
#include "game/physics/mob_ai.hpp"

std::atomic_bool PlayerController::shooting = false;

void PlayerController::shoot(const Input& input, const Camera& camera) {
	const auto mouseCoord = camera.fromScreenToMap(input.getMouseCoord());
	aimCoord.store(mouseCoord, std::memory_order_relaxed);

	const bool flag = input.active(Shoot);
	shooting.store(flag, std::memory_order_relaxed);
}

void PlayerController::mine() {

}

void PlayerController::move(const Input& input, Camera& camera, const float tickOfset) {
	if (guiActive)
		return;
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
		camera.setPosition(targetedMob->position + targetedMob->velocity * tickOfset);
	}
	camera.scale(input);
}

void PlayerController::update(Engine& engine, const float tickOfset) {
	const Input& input = engine.getMainWindow().getInput();
	Camera& camera = engine.getCamera();
	const GUI& gui = engine.getGUI();

	guiActive = gui.hasOverlaped() || !gui.isMouseFree();
	move(engine.getMainWindow().getInput(), camera, tickOfset);
	shoot(input, camera);
	mine();
	captureMob(input, camera);
}

void PlayerController::captureMob(const Input& input, const Camera& camera) {
	if (!input.jactive(Control_unit))
		return;
	for (const auto& mob : playerTeam->getMobs()) {
		if (t1::areCloser(camera.fromMapToScreen(mob.position), input.getMouseCoord(), 20.f))
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
