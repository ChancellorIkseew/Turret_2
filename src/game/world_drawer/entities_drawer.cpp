#include "entities_drawer.hpp"
//
#include "game/physics/mobs_system.hpp"
#include "game/physics/shells_system.hpp"
#include "game/physics/turrets_system.hpp"

void EntitiesDrawer::draw(const Camera& camera, const Renderer& renderer, MobSoA& mobs, const ShellSoA& shells) {
    mobs::drawMobs(mobs, camera, renderer);
    turrets::drawTurrets(mobs, camera, renderer);
    shells::drawShells(shells, camera, renderer);
}
