#pragma once
#include <atomic>
#include <cstdint>
#include "engine/coords/pixel_coord.hpp"

struct Mob;
class Camera;
class Engine;
class GUI;
class Input;
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
	static std::atomic_bool shooting;

	static void move(const Input& input, Camera& camera, const float tickOfset);
	static void mine();
	static void shoot(const Input& input, const Camera& camera);
public:
	static void captureMob(const Team& playerTeam, const Input& input, const Camera& camera);
	static void update(const Team& playerTeam, Engine& engine, const float tickOfset);

	static void resetTarget();
	static void setTarget(const Mob& mob);
	static Mob* getTarget() { return targetedMob; }

	static PixelCoord getMotionVector() { return motionVector.load(std::memory_order_relaxed); }
	static PixelCoord getAimCoord() { return aimCoord.load(std::memory_order_relaxed); }
	static bool shootingActive() { return shooting.load(std::memory_order_relaxed); }

};
