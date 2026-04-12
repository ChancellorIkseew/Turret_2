#include "mob_ai.hpp"

MovingAI mob_ai::getMovingAI(const std::string& name) {
    if (name == "none") return MovingAI::none;
    if (name == "basic") return MovingAI::basic;
    if (name == "player_controlled") return MovingAI::player_controlled;
    return MovingAI::none;
}

ShootingAI mob_ai::getShootingAI(const std::string& name) {
    if (name == "none") return ShootingAI::none;
    if (name == "basic") return ShootingAI::basic;
    if (name == "player_controlled") return ShootingAI::player_controlled;
    return ShootingAI::none;
}
