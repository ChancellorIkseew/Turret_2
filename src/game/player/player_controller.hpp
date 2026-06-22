#pragma once
#include <cstdint>
#include "game/entities/mob_manager.hpp"

class Camera;
class Engine;
class Input;
class Presets;
class Team;

class PlayerController {
    enum class State : uint8_t { control_camera, control_mob };
    State state = State::control_camera;
    Team* playerTeam = nullptr;
    PixelCoord motionVector;
    PixelCoord aimCoord;
    bool shooting = false;
public:
    void update(const Input& input, Camera& camera, const bool paused, MobManager& mobs, const Presets& presets);

    void setPlayerTeam(Team* team) { playerTeam = team; }
    Team* getPlayerTeam() { return playerTeam; }

    PixelCoord getMotionVector() const { return motionVector; }
    PixelCoord getAimCoord()     const { return aimCoord; }
    bool shootingActive()        const { return shooting; }
private:
    void move(const Input& input, Camera& camera, const MobManager& mobs, const bool isPaused);
    void mine();
    void shoot(const Input& input, const Camera& camera);
    void captureMob(const Input& input, const Camera& camera, MobManager& mobs, const Presets& presets);
};
