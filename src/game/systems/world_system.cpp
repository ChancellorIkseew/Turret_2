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
#include "game/world_drawer/entities_drawer.hpp"
#include "game/world_drawer/particles_drawer.hpp"
#include "game/world_drawer/world_drawer.hpp"

void drawInfoOnCursor(Renderer& renderer, const Camera& camera, const Presets& presets, BlockMap& blocks, const TileCoord targetTile);

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
    construction::buildBlueprints(mobs.getSoa(), presets, world.getBlueprints(), scripts, blocks);
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
    const uint64_t timeMs = engine.getMainWindow().getTimeMs();
    const TileCoord targetTile = t1::tile(camera.fromScreenToMap(engine.getMainWindow().getInput().getMouseCoord()));
    //
    renderer.setView(camera.getMapScale(), camera.getTranslation());
    //renderer.setShaderProgram(*shaders.lightingShader);
    // Draw lifthing here, before other. Now ligthing is not needed.
    //
    renderer.setShaderProgram(*shaders.baseShader);
    drawer.drawMap(camera, renderer, world.getMap());
    //
    renderer.setShaderProgram(*shaders.squareShadowShader);
    drawer.drawBlockShadows(world.getBlocks(), camera, renderer);
    //
    renderer.setShaderProgram(*shaders.baseShader);
    drawer.drawBlocks(world.getBlocks(), renderer, presets);
    world.getBlueprints().drawGhosts(renderer, presets, timeMs);
    //
    renderer.setShaderProgram(*shaders.emergeShader);
    world.getBlueprints().drawInProgress(renderer, presets);
    //
    renderer.setShaderProgram(*shaders.baseShader);
    engine.getGUI().drawDiegeticElements(renderer);
    drawEntities(camera, renderer, world.getBlocks(), world.getMobs().getSoa(), world.getShells().getSoa(), presets, tickCount);
    drawShardParticles(camera, renderer, world.getParticles().getSoa());
    drawInfoOnCursor(renderer, camera, presets, world.getBlocks(), targetTile);
    //
    renderer.setShaderProgram(*shaders.shieldShader);
    mobs::drawMobShields(world.getMobs().getSoa(), presets, camera, renderer, tickCount);
    renderer.setShaderProgram(*shaders.additiveLightShader);
    shells::drawShellsLighting(world.getShells().getSoa(), presets, camera, renderer);
    drawLightParticles(camera, renderer, world.getParticles().getSoa());
}

void drawInfoOnCursor(Renderer& renderer, const Camera& camera, const Presets& presets, BlockMap& blocks, const TileCoord targetTile) {
    if (!blocks.contains(targetTile) || blocks.at(targetTile).type != BlockType::turret)
        return;
    const TurretSoA& turrets = blocks.getMeta().getTurrets().getSoa();
    for (size_t i = 0; i < turrets.turretCount; ++i) {
        if (t1::tile(turrets.position[i]) != targetTile)
            continue;
        const float range = presets.getTurret(turrets.preset[i]).range;
        Blueprints::drawRange(renderer, turrets.position[i], range);
        break;
    }
}
