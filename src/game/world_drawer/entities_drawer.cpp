#include "entities_drawer.hpp"
//
#include "game/physics/mobs_system.hpp"
#include "game/physics/shells_system.hpp"
#include "game/physics/turrets_system.hpp"

void EntitiesDrawer::draw(const Camera& camera, const Renderer& renderer,
    MobSoA& mobs, const ShellSoA& shells, const Presets& presets, const uint64_t tickCount) {
    mobs::drawMobs(mobs, presets, camera, renderer, tickCount);
    turrets::drawTurrets(mobs, presets, camera, renderer);
    shells::drawShells(shells, presets, camera, renderer);
}
