#include "entities_drawer.hpp"
//
#include "game/physics/mobs_system.hpp"
#include "game/physics/shells_system.hpp"
#include "game/physics/turrets_system.hpp"

void EntitiesDrawer::draw(const Camera& camera, MobSoA& mobs, const ShellSoA& shells) {
    mobs::drawMobs(mobs, camera);
    turrets::drawTurrets(mobs, camera);
    shells::drawShells(shells, camera);
}
