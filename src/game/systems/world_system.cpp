#include "world_system.hpp"
//
#include "engine/assets/assets.hpp"
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
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

void drawUnitSelected(Renderer& renderer, const MobSoA& mobs, const TurretSoA& turrets,
    const Presets& presets, const PlayerController::Unit selected);

void world::update(World& world, const Camera& camera, const Presets& presets, const uint64_t timeMs, const uint64_t tickCount,
    const PlayerController& playerController, SoundQueue& worldSounds, BuiltInScripts& scripts) {
    //
    auto& blocks = world.getBlocks();
    auto& chunks = world.getChunks();
    auto& mobs = world.getMobs();
    auto& shells = world.getShells();
    auto& schematic = world.getSchematic();
    auto& particles = world.getParticles();
    auto mobTurrets = fromMobs(mobs.getSoa());
    auto blockTurrets = fromBlocks(blocks.getMeta().getTurrets().getSoa());
    //
    world.getBuildBems().clear();
    chunks.update(mobs.getSoa());
    blocks.updateBlocks(world.getMap(), presets, world.getTeams(), tickCount);
    shells::processShells(world, presets, worldSounds, camera, tickCount);
    mobs::processMobs(mobs.getSoa(), chunks, blocks, presets, particles, camera);
    ai::updateMovingAI(mobs.getSoa(), presets, playerController, schematic, blocks);
    ai::updateShootingAI(blockTurrets, mobs.getSoa(), blocks, presets, playerController);
    ai::updateShootingAI(mobTurrets, mobs.getSoa(), blocks, presets, playerController);
    turrets::processTurrets(blockTurrets, shells, particles, presets, worldSounds, camera, timeMs);
    turrets::processTurrets(mobTurrets, shells, particles, presets, worldSounds, camera, timeMs);
    particles::updateParticles(particles);
    // Build when spans are used and can be spoiled.
    schematic.syncWithWorld(blocks);
    construction::buildBlueprints(mobs.getSoa(), presets, schematic, blocks,
        world.getBuildBems(), world.getTeams(), worldSounds, particles);
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
    world.getSchematic().drawGhosts(renderer, presets, timeMs);
    //
    renderer.setShaderProgram(*shaders.emergeShader);
    drawer.drawBlocksInProgress(world.getBlocks(), renderer, presets);
    //
    renderer.setShaderProgram(*shaders.baseShader);
    engine.getGUI().drawDiegeticElements(renderer);
    drawEntities(camera, renderer, world.getBlocks(), world.getMobs().getSoa(), world.getShells().getSoa(), presets, tickCount);
    drawShardParticles(camera, renderer, world.getParticles().getSoa());
    drawInfoOnCursor(renderer, camera, presets, world.getBlocks(), targetTile);
    //
    renderer.setShaderProgram(*shaders.shieldShader);
    mobs::drawMobShields(world.getMobs().getSoa(), presets, camera, renderer, tickCount);
    //
    renderer.setShaderProgram(*shaders.smokeShader);
    drawSmokeParticles(camera, renderer, world.getParticles().getSoa());
    //
    renderer.setShaderProgram(*shaders.additiveLightShader);
    shells::drawShellsLighting(world.getShells().getSoa(), presets, camera, renderer);
    drawLightParticles(camera, renderer, world.getParticles().getSoa());
    //
    renderer.setShaderProgram(*shaders.buildBeamShader);
    world.getBuildBems().draw(renderer, tickCount);
    //
    renderer.setShaderProgram(*shaders.monochromeShader);
    const PlayerController::Unit unitSelected = engine.getSession().getPlayerController().getUnitSelected();
    drawUnitSelected(renderer, world.getMobs().getSoa(), world.getBlocks().getMeta().getTurrets().getSoa(), presets, unitSelected);
}

void drawInfoOnCursor(Renderer& renderer, const Camera& camera, const Presets& presets, BlockMap& blocks, const TileCoord targetTile) {
    if (!blocks.contains(targetTile) || (blocks.at(targetTile).type != BlockType::turret && blocks.at(targetTile).type != BlockType::link))
        return;
    const TurretSoA& turrets = blocks.getMeta().getTurrets().getSoa();
    const TileCoord masterTile = blocks.getMaster(targetTile);
    for (size_t i = 0; i < turrets.turretCount; ++i) {
        if (turrets.masterTile[i] != masterTile)
            continue;
        const float range = presets.getTurret(turrets.preset[i]).range;
        Schematic::drawRange(renderer, turrets.position[i], range);
        break;
    }
}

void drawUnitSelected(Renderer& renderer, const MobSoA& mobs, const TurretSoA& turrets,
    const Presets& presets, const PlayerController::Unit selected) {
    constexpr uint32_t COLOR = cl::BEIGE_TR | 0x80;
    if (selected.mob) {
        const size_t i = selected.mob.value();
        const auto& preset = presets.getMob(mobs.preset[i]);
        const auto& visual = preset.visual;
        const auto& tVisual = presets.getTurret(preset.turret).visual;

        const uint8_t frame = mobs.chassisTick[i] / visual.frameTicks;
        TextureRect frameTextureRect = visual.textureRect;
        frameTextureRect.h = visual.frameHeight;
        frameTextureRect.y += static_cast<float>(visual.frameOrder[frame]) * visual.frameHeight;

        renderer.draw(frameTextureRect, mobs.position[i], visual.size, visual.origin, t1::PI - mobs.angle[i], COLOR);
        renderer.draw(tVisual.textureRect, mobs.position[i], tVisual.size, tVisual.origin, t1::PI - mobs.turretAngle[i], COLOR);
    }
    if (selected.turret) {
        const size_t i = selected.turret.value();
        const PixelCoord recoilVector(std::sin(turrets.angle[i]), std::cos(turrets.angle[i]));
        const PixelCoord position = turrets.position[i] - recoilVector * turrets.currentRecoil[i];
        const auto& visual = presets.getTurret(turrets.preset[i]).visual;

        renderer.draw(visual.textureRect, position, visual.size, visual.origin, t1::PI - turrets.angle[i], COLOR);
    }
}
