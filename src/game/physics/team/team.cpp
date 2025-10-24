#include "team.hpp"
//
#include "game/physics/mobs_system.hpp"
#include "game/physics/shells_system.hpp"
#include "game/world/world.hpp"
#include "game/physics/mob_ai.hpp"

void Team::spawnMob(const MobPreset& preset, const PixelCoord position, const float angle) {
    //mobs.emplace_back(std::make_unique<PlayerControlledMoving>(), nullptr, preset, position, angle, Team::ID);
    mobs.emplace_back(std::make_unique<BasicMovingAI>(), nullptr, preset, position, angle, Team::ID);
    mobs.back().movingAI->setDest(PixelCoord(100.0f, 320.0f));
}
void Team::spawnShell(const ShellPreset& preset, const PixelCoord position, const float angle) {
    shells.push_back(Shell(preset, position, angle, Team::ID));
}

void Team::interact(World& world) {
    shells::processShells(shells, world.getTeams());
    mobs::processMobs(mobs, world.getTeams());
}

void Team::drawGroundUnits(const Camera& camera, const float tickOfset) {
    mobs::drawMobs(mobs, camera, tickOfset);
}

void Team::drawAirUnits(const Camera& camera, const float tickOfset) {
    // TODO : implement
}

void Team::drawShells(const Camera& camera, const float tickOfset) {
    shells::drawShells(shells, camera);
}
