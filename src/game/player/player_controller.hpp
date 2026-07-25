#pragma once
#include <cstdint>
#include "game/common/physics_base.hpp"

class Camera;
class Input;
class Presets;
struct MobSoA;
struct TurretSoA;

class PlayerController {
    TeamID m_playerTeamID = 0;
    PixelCoord m_motionVector;
    PixelCoord m_aimCoord;
    bool m_shooting = false, m_holdsBlock = false;
public:
    void update(const Input& input, Camera& camera, const bool paused, MobSoA& mobs, TurretSoA& turrets, const Presets& presets);
    
    void setHoldsBlock(const bool flag) { m_holdsBlock = flag; }
    void setPlayerTeamID(const TeamID teamID) { m_playerTeamID = teamID; }
    
    bool       holdsBlock()      const noexcept { return m_holdsBlock; }
    TeamID     getPlayerTeamID() const noexcept { return m_playerTeamID; }
    PixelCoord getMotionVector() const noexcept { return m_motionVector; }
    PixelCoord getAimCoord()     const noexcept { return m_aimCoord; }
    bool       isShooting()      const noexcept { return m_shooting; }
private:
    void move(const Input& input);
    void mine();
    void shoot(const Input& input, const Camera& camera);
    void captureMobOrTurret(const Input& input, const Camera& camera, MobSoA& mobs, TurretSoA& turrets,
        const std::optional<size_t> mob, const std::optional<size_t> turret, const Presets& presets) const;
    void moveCamera(const MobSoA& mobs, const std::optional<size_t> mob, const bool paused, Camera& camera, const Input& input) const;
};
