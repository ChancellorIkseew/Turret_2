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

	State state = State::control_camera;
	bool guiActive;

	Team* playerTeam = nullptr;
	Mob* targetedMob = nullptr;
	std::atomic<PixelCoord> motionVector;
	std::atomic<PixelCoord> aimCoord;
	std::atomic_bool shooting = false;

	void move(const Input& input, Camera& camera, const float tickOfset);
	void mine();
	void shoot(const Input& input, const Camera& camera);
public:
	PlayerController() = default;

	void captureMob(const Input& input, const Camera& camera);
	void update(Engine& engine, const float tickOfset);

	void setPlayerTeam(Team* team) { playerTeam = team; }
	Team* getPlayerTeam() { return playerTeam; }
	void resetTarget();
	void setTarget(const Mob& mob);
	Mob* getTarget() { return targetedMob; }

	PixelCoord getMotionVector() const { return motionVector.load(std::memory_order_relaxed); }
	PixelCoord getAimCoord()     const     { return aimCoord.load(std::memory_order_relaxed); }
	bool shootingActive()        const     { return shooting.load(std::memory_order_relaxed); }
};
