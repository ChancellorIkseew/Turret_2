#pragma once
#include <cstdint>
#include "game/common/physics_base.hpp"

class Camera;
class Input;
class Presets;
struct MobSoA;
struct TurretSoA;

class PlayerController {
    TeamID playerTeamID = 0;
    PixelCoord motionVector;
    PixelCoord aimCoord;
    bool shooting = false, holdsBlock = false;
public:
    void update(const Input& input, Camera& camera, const bool paused, MobSoA& mobs, TurretSoA& turrets, const Presets& presets);

    void setHoldsBlock(const bool flag) { holdsBlock = flag; }
    void setPlayerTeamID(const TeamID teamID) { playerTeamID = teamID; }
    TeamID getPlayerTeamID() const { return playerTeamID; }

    PixelCoord getMotionVector() const { return motionVector; }
    PixelCoord getAimCoord()     const { return aimCoord; }
    bool shootingActive()        const { return shooting; }
private:
    void move(const Input& input);
    void mine();
    void shoot(const Input& input, const Camera& camera);
    void captureMobOrTurret(const Input& input, const Camera& camera, MobSoA& mobs, TurretSoA& turrets,
        const std::optional<size_t> mob, const std::optional<size_t> turret, const Presets& presets) const;
    void moveCamera(const MobSoA& mobs, const std::optional<size_t> mob, const bool paused, Camera& camera, const Input& input) const;
};
