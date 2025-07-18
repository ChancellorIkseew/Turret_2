#pragma once
#include <atomic>
#include "engine/coords/pixel_coord.hpp"

struct Mob;
class Camera;
class Team;

class MobController {
	static inline Mob* targetedMob = nullptr;
	//static inline Control preveousControlType = Control::NONE;
	static inline std::atomic<PixelCoord> motionVector;
	static inline std::atomic<PixelCoord> aimCoord;
	static inline std::atomic<bool> shooting;

	static void move();
	static void mine();
	static void shoot(const Camera& camera);
public:
	static void captureMob(Mob* mob);
	static void interact(const Team& player, const Camera& camera);

	static void setTarget(Mob* mob) { targetedMob = mob; }
	static Mob* getTarget() { return targetedMob; }

	static PixelCoord getMotionVector() { return motionVector.load(std::memory_order_relaxed); }
	static PixelCoord getAimCoord() { return aimCoord.load(std::memory_order_relaxed); }
	static bool shootingActive() { return shooting.load(std::memory_order_relaxed); }

};
