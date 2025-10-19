#pragma once
#include <atomic>
#include "engine/coords/pixel_coord.hpp"

struct Mob;
class Camera;
class Team;

class MobController {
	enum class State {
		control_camera,
		control_mob,
		use_gui
	};

	static inline State state = State::control_camera;

	static inline Mob* targetedMob = nullptr;
	//static inline Control preveousControlType = Control::NONE;
	static inline std::atomic<PixelCoord> motionVector;
	static inline std::atomic<PixelCoord> aimCoord;
	static inline std::atomic<bool> shooting;

	static void move(Camera& camera);
	static void mine();
	static void shoot(const Camera& camera);
public:
	static void captureMob(const Team& playerTeam, const Camera& camera);
	static void update(const Team& playerTeam, Camera& camera);

	static void setTarget(Mob* mob) { targetedMob = mob; }
	static Mob* getTarget() { return targetedMob; }

	static PixelCoord getMotionVector() { return motionVector.load(std::memory_order_relaxed); }
	static PixelCoord getAimCoord() { return aimCoord.load(std::memory_order_relaxed); }
	static bool shootingActive() { return shooting.load(std::memory_order_relaxed); }

};
