#pragma once
#include <atomic>
#include <cstdint>
#include "engine/coords/pixel_coord.hpp"

struct Mob;
class GUI;
class Camera;
class Team;

class PlayerController {
	enum class State : uint8_t {
		control_camera,
		control_mob
	};

	static inline State state = State::control_camera;
	static inline bool guiActive;

	static inline Mob* targetedMob = nullptr;
	static inline std::atomic<PixelCoord> motionVector;
	static inline std::atomic<PixelCoord> aimCoord;
	static inline std::atomic<bool> shooting;

	static void move(Camera& camera, const float tickOfset);
	static void mine();
	static void shoot(const Camera& camera);
public:
	static void captureMob(const Team& playerTeam, const Camera& camera);
	static void update(const Team& playerTeam, Camera& camera, const GUI& gui, const float tickOfset);

	static void resetTarget();
	static void setTarget(const Mob& mob);
	static Mob* getTarget() { return targetedMob; }

	static PixelCoord getMotionVector() { return motionVector.load(std::memory_order_relaxed); }
	static PixelCoord getAimCoord() { return aimCoord.load(std::memory_order_relaxed); }
	static bool shootingActive() { return shooting.load(std::memory_order_relaxed); }

};
