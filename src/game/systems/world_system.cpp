#include "world_system.hpp"
//
#include "engine/assets/assets.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "game/systems/ai_system.hpp"
#include "game/systems/construction_system.hpp"
#include "game/systems/mobs_system.hpp"
#include "game/systems/particles_system.hpp"
#include "game/systems/shells_system.hpp"
#include "game/systems/turrets_system.hpp"
#include "game/systems/turret_components.hpp"
#include "game/world/world.hpp"
#include "game/world_drawer/particles_drawer.hpp"
#include "game/world_drawer/world_drawer.hpp"

void world::update(World& world, const Camera& camera, const Presets& presets, const uint64_t timeMs,
    const PlayerController& playerController, SoundQueue& worldSounds, BuiltInScripts& scripts) {
    //
    auto& blocks = world.getBlocks();
    auto& chunks = world.getChunks();
    auto& mobs = world.getMobs();
    auto& shells = world.getShells();
    auto& particles = world.getParticles();
    auto mobTurrets = fromMobs(mobs.getSoa());
    auto blockTurrets = fromBlocks(blocks.getMeta().getTurrets().getSoa());
    //
    chunks.update(mobs.getSoa());
    updateBlocks(blocks, world.getMap(), presets);
    shells::processShells(world, presets, worldSounds, camera);
    mobs::processMobs(mobs.getSoa(), chunks, blocks, presets);
    ai::updateMovingAI(mobs.getSoa(), presets, playerController, world.getBlueprints());
    ai::updateShootingAI(blockTurrets, mobs.getSoa(), blocks, presets, playerController);
    ai::updateShootingAI(mobTurrets, mobs.getSoa(), blocks, presets, playerController);
    turrets::processTurrets(blockTurrets, shells, particles, presets, worldSounds, camera, timeMs);
    turrets::processTurrets(mobTurrets, shells, particles, presets, worldSounds, camera, timeMs);
    particles::updateParticles(particles);
    // Build when spans are used and can be spoiled.
    construction::buildBlueprints(mobs.getSoa(), presets, world.getBlueprints(), scripts);
    // Clean up only after all processing.
    shells::cleanupShells(shells, presets);
    mobs::cleanupMobs(mobs, presets);
    blocks.getMeta().cleanUp();
}

void world::draw(World& world, Renderer& renderer, WorldDrawer& drawer, const Camera& camera, const Assets& assets,
    const uint64_t tickCount, Engine& engine) {
    //
    const Presets& presets = assets.getPresets();
    const Shaders& shaders = assets.getShaders();
    auto& blocksDrawer = drawer.getBlocksDrawer();

    renderer.setShaderProgram(*shaders.lightingShader);
    renderer.setView(camera.getMapScale(), camera.getTranslation());
    //drawLightParticles(camera, renderer, world.getParticles().getSoa());
    //
    renderer.setShaderProgram(*shaders.baseShader);
    drawer.draw(camera, renderer, world, assets.getPresets(), assets, tickCount);
    //
    renderer.setShaderProgram(*shaders.squareShadowShader);
    blocksDrawer.drawShadows(world.getBlocks(), assets, camera, renderer);
    //
    renderer.setShaderProgram(*shaders.baseShader);
    blocksDrawer.draw(world.getBlocks(), assets, camera, renderer);
    drawShardParticles(camera, renderer, world.getParticles().getSoa());
    engine.getGUI().drawDiegeticElements(renderer);           // temporary update will be related with blueprints
    world.getBlueprints().drawGhosts(renderer, engine);
    renderer.setShaderProgram(*shaders.emergeShader);
    world.getBlueprints().drawInProgress(renderer, engine); // temporary
    //
    renderer.setShaderProgram(*shaders.shieldShader);
    mobs::drawMobShields(world.getMobs().getSoa(), presets, camera, renderer, tickCount);
    renderer.setShaderProgram(*shaders.additiveLightShader);
    shells::drawShellsLighting(world.getShells().getSoa(), presets, camera, renderer);
    drawLightParticles(camera, renderer, world.getParticles().getSoa());
}
