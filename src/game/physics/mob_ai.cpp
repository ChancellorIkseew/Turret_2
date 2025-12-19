#include "mob_ai.hpp"

MobMovingAI mob_ai::getMovingAI(const std::string& name) {
    if (name == "basic") return MobMovingAI::basic;
    if (name == "player_controlled") return MobMovingAI::player_controlled;
    return MobMovingAI::none;
}

MobShootingAI mob_ai::getShootingAI(const std::string& name) {
    if (name == "basic") return MobShootingAI::basic;
    if (name == "player_controlled") return MobShootingAI::player_controlled;
    return MobShootingAI::none;
}
