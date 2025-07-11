#include "team.hpp"
//
#include "game/physics/shells_system.hpp"

void Team::spawnShell(const ShellPreset& preset, const PixelCoord position, const float angle) {
    shells.push_back(Shell(preset, position, angle, Team::ID));
}

void Team::interact(const World& world) {
    //TODO: shells::processShells(shells, world->getTeams());
    //TODO: mobs::processMobs(mobs, world->getTeams());
}

void Team::draw(const Camera& camera) const {
    shells::drawShells(shells, camera);
    //TODO: mobs::drawMobs(mobs, camera);
}
