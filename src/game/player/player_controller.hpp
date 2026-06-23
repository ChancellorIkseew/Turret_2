#pragma once
#include <cstdint>
#include "game/common/physics_base.hpp"

class Camera;
class Engine;
class Input;
class Presets;
struct MobSoA;
struct TurretSoA;

class PlayerController {
    enum class State : uint8_t { control_camera, control_mob, control_turret };
    State state = State::control_camera;
    TeamID playerTeamID = 0;
    PixelCoord motionVector;
    PixelCoord aimCoord;
    bool shooting = false;
public:
    void update(const Input& input, Camera& camera, const bool paused, MobSoA& mobs, TurretSoA& turrets, const Presets& presets);

    void setPlayerTeamID(const TeamID teamID) { playerTeamID = teamID; }
    TeamID getPlayerTeamID() const { return playerTeamID; }

    PixelCoord getMotionVector() const { return motionVector; }
    PixelCoord getAimCoord()     const { return aimCoord; }
    bool shootingActive()        const { return shooting; }
private:
    void move(const Input& input, Camera& camera, MobSoA& mobs, const bool isPaused);
    void mine();
    void shoot(const Input& input, const Camera& camera);
    void captureMob(const Input& input, const Camera& camera, MobSoA& mobs, TurretSoA& turrets, const Presets& presets);
    void sinc(const MobSoA& mobs, const TurretSoA& turrets);
};
