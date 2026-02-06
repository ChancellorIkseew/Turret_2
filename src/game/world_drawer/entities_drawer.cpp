#include "entities_drawer.hpp"
//
#include "engine/render/renderer.hpp"
#include "game/physics/mobs_system.hpp"
#include "game/physics/shells_system.hpp"
#include "game/physics/turrets_system.hpp"

void EntitiesDrawer::draw(const Camera& camera, Renderer& renderer,
    MobSoA& mobs, const ShellSoA& shells, const Presets& presets, const uint64_t tickCount) {
    renderer.setScaleMode(ScaleMode::pixelart);
    mobs::drawMobs(mobs, presets, camera, renderer, tickCount);
    turrets::drawTurrets(mobs, presets, camera, renderer);
    shells::drawShells(shells, presets, camera, renderer);
    renderer.setScaleMode(ScaleMode::nearest);
}
