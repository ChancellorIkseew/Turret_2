#pragma once
#include <atomic>
#include <cstdint>
#include "game/physics/mob_manager.hpp"

class Camera;
class Engine;
class GUI;
class Input;
class Team;

class PlayerController {
    enum class State : uint8_t {
        control_camera,
        control_mob,
        no_control
    };

    State state = State::control_camera;

    Team* playerTeam = nullptr;
    MobID targetMobID = IDManager<MobID>::INVALID_ID;
    std::atomic<PixelCoord> motionVector;
    std::atomic<PixelCoord> aimCoord;
    std::atomic_bool shooting = false;

    void move(const Input& input, Camera& camera, const MobManager& mobs, const float tickOfset);
    void mine();
    void shoot(const Input& input, const Camera& camera);
public:
    PlayerController() = default;

    void captureMob(const Input& input, const Camera& camera, MobManager& mobs);
    void update(Engine& engine, MobManager& mobs, const float tickOfset);

    void setPlayerTeam(Team* team) { playerTeam = team; }
    Team* getPlayerTeam() { return playerTeam; }
    void resetTarget(MobManager& mobs);
    void setTarget(MobManager& mobs, const MobID mobID);
    MobID getTarget() { return targetMobID; }

    PixelCoord getMotionVector() const { return motionVector.load(std::memory_order_relaxed); }
    PixelCoord getAimCoord()     const     { return aimCoord.load(std::memory_order_relaxed); }
    bool shootingActive()        const     { return shooting.load(std::memory_order_relaxed); }
};
