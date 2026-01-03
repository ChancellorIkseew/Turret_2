#include "entities_drawer.hpp"
//
#include "game/physics/mobs_system.hpp"
#include "game/physics/shells_system.hpp"
#include "game/physics/turrets_system.hpp"

void EntitiesDrawer::draw(const Camera& camera, const MobSoA& mobs, const ShellSoA& shells, const float tickOfset) {
    mobs::drawMobs(mobs, camera, tickOfset);
    turrets::drawTurrets(mobs, camera, tickOfset);
    shells::drawShells(shells, camera, tickOfset);
}
