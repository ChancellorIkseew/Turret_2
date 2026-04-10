#include "entities_drawer.hpp"
//
#include "engine/render/renderer.hpp"
#include "game/systems/mobs_system.hpp"
#include "game/systems/shells_system.hpp"
#include "game/systems/turrets_system.hpp"
#include "game/blocks/block_map.hpp"
#include "game/systems/turret_components.hpp"

void EntitiesDrawer::draw(const Camera& camera, Renderer& renderer,
    BlockMap& blocks, MobSoA& mobs, const ShellSoA& shells, const Presets& presets, const uint64_t tickCount) {
    renderer.setScaleMode(ScaleMode::pixelart);
    mobs::drawMobs(mobs, presets, camera, renderer, tickCount);
    turrets::drawTurrets(fromBlocks(blocks.getMeta().getTurrets().getSoa()), presets, camera, renderer);
    turrets::drawTurrets(fromMobs(mobs), presets, camera, renderer);
    shells::drawShells(shells, presets, camera, renderer);
    renderer.setScaleMode(ScaleMode::nearest);
}
