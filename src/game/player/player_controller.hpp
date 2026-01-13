#pragma once
#include <cstdint>
#include "game/physics/mob_manager.hpp"

class Camera;
class Engine;
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
public:
    PlayerController() = default;

    void update(const Input& input, Camera& camera, const bool paused, MobManager& mobs, const Presets& presets);

    void setPlayerTeam(Team* team) { playerTeam = team; }
    Team* getPlayerTeam() { return playerTeam; }
    void resetTarget(MobManager& mobs, const Presets& presets);
    void setTarget(MobManager& mobs, const MobID mobID, const Presets& presets);
    MobID getTarget() const { return targetMobID; }

    PixelCoord getMotionVector() const { return motionVector; }
    PixelCoord getAimCoord()     const { return aimCoord; }
    bool shootingActive()        const { return shooting; }
private:
    void move(const Input& input, Camera& camera, const MobManager& mobs, const bool isPaused);
    void mine();
    void shoot(const Input& input, const Camera& camera);
    void captureMob(const Input& input, const Camera& camera, MobManager& mobs, const Presets& presets);
};
