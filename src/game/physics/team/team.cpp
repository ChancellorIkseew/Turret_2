#include "team.hpp"
//
#include "game/physics/mobs_system.hpp"
#include "game/physics/shells_system.hpp"
#include "game/world/world.hpp"
#include "game/physics/mob_ai.hpp"

void Team::spawnMob(const MobPreset& preset, const PixelCoord position, const float angle) {
    mobs.emplace_back(std::make_unique<PlayerControlledMoving>(), nullptr, preset, position, angle, Team::ID);
}
void Team::spawnShell(const ShellPreset& preset, const PixelCoord position, const float angle) {
    shells.push_back(Shell(preset, position, angle, Team::ID));
}

void Team::interact(World& world) {
    shells::processShells(shells, world.getTeams());
    mobs::processMobs(mobs, world.getTeams());
}

void Team::drawGroundUnits(const Camera& camera) const {
    mobs::drawMobs(mobs, camera);
}

void Team::drawAirUnits(const Camera& camera) const {
    // TODO : implement
}

void Team::drawShells(const Camera& camera) const {
    shells::drawShells(shells, camera);
}
