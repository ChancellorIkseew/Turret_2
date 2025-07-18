#include "player_controller.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/window/input/input.hpp"
#include "engine/util/sleep.hpp"
#include "game/world/camera.hpp"

void MobController::shoot(const Camera& camera) {
	const auto mouse = camera.fromScreenToMap(Input::getMouseCoord());
	aimCoord.store(PixelCoord(mouse.x, mouse.y), std::memory_order_relaxed);

	const bool flag = Input::active(Shoot);
	shooting.store(flag, std::memory_order_relaxed);
}

void MobController::mine() {

}

void MobController::move() {
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
}


void MobController::interact(const Team& player, const Camera& camera) {
	move();
	shoot(camera);
	mine();
}

void MobController::captureMob(Mob* mob) {
	targetedMob = mob;
}