#pragma once
#include <cstdint>
#include "game/physics/mob_manager.hpp"

class Camera;
class Engine;
class GUI;
class Input;
class Team;

class PlayerController {
    enum class State : uint8_t { control_camera, control_mob };
    State state = State::control_camera;

    Team* playerTeam = nullptr;
    MobID targetMobID = IDManager<MobID>::INVALID_ID;
    PixelCoord motionVector;
    PixelCoord aimCoord;
    std::atomic_bool shooting = false;

    void move(const Input& input, Camera& camera, const MobManager& mobs, const bool isPaused);
    void mine();
    void shoot(const Input& input, const Camera& camera);
public:
    PlayerController() = default;

    void captureMob(const Input& input, const Camera& camera, MobManager& mobs);
    void update(Engine& engine, MobManager& mobs);

    void setPlayerTeam(Team* team) { playerTeam = team; }
    Team* getPlayerTeam() { return playerTeam; }
    void resetTarget(MobManager& mobs);
    void setTarget(MobManager& mobs, const MobID mobID);
    MobID getTarget() { return targetMobID; }

    PixelCoord getMotionVector() const { return motionVector; }
    PixelCoord getAimCoord()     const { return aimCoord; }
    bool shootingActive()        const { return shooting; }
};
