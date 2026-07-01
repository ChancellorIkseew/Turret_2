#include "game_session.hpp"
//
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/scripting/scripting.hpp"
#include "engine/settings/settings.hpp"
#include "game/particles/particles_system.hpp"
#include "game/systems/ai_system.hpp"
#include "game/systems/construction_system.hpp"
#include "game/systems/mobs_system.hpp"
#include "game/systems/shells_system.hpp"
#include "game/systems/turrets_system.hpp"
#include "game/systems/turret_components.hpp"
#include "game/world/world.hpp"
#include "game/events/events.hpp"
#include "game/world_drawer/particles_drawer.hpp"

// Constuctor and destructor in cpp are needed for forward declaraton "GUI" and "World" classes in hpp.
GameSession::GameSession(std::unique_ptr<World> world, std::unique_ptr<GUI> gui, const Assets& assets,
    const bool paused, const GameMode gameMode) :
    camera(world->getMap().getSize(), Settings::gameplay.cameraInertia), world(std::move(world)), gui(std::move(gui)),
    worldDrawer(assets), pausedManually(paused), timeCount(0, 10800), builtInScripts(assets, *this->world), gameMode(gameMode) {
    prepare(assets.getPresets());
}
GameSession::~GameSession() = default;

void GameSession::prepare(const Presets& presets) {
    Team* playerTeam = world->getTeams().addTeam(U"player");
    Team* enemyTeam  = world->getTeams().addTeam(U"enemy");
    playerController.setPlayerTeamID(playerTeam->getID());
    TeamID playerTeamID = playerTeam->getID();
    // temporary for alpha-test
    const TileCoord corePosition = world->getMap().getSize() / 2;
    builtInScripts.placeBlock(presets.getBlockID("core"), corePosition, playerTeamID, BlockRot::up);
}

void GameSession::updateSimulation(const Presets& presets, Engine& engine) {
    auto& blocks    = world->getBlocks();
    auto& chunks    = world->getChunks();
    auto& mobs      = world->getMobs();
    auto& shells    = world->getShells();
    auto& particles = world->getParticles();
    auto mobTurrets   = fromMobs(mobs.getSoa());
    auto blockTurrets = fromBlocks(blocks.getMeta().getTurrets().getSoa());
    //
    chunks.update(mobs.getSoa());
    updateBlocks(blocks, world->getMap(), presets);
    shells::processShells(shells.getSoa(), mobs.getSoa(), chunks, blocks);
    mobs::processMobs(mobs.getSoa(), chunks, blocks, presets);
    ai::updateMovingAI(mobs.getSoa(), presets, playerController, world->getBlueprints());
    ai::updateShootingAI(blockTurrets, mobs.getSoa(), blocks, presets, playerController);
    ai::updateShootingAI(mobTurrets, mobs.getSoa(), blocks, presets, playerController);
    turrets::processTurrets(blockTurrets, shells, particles, presets, worldSounds, camera);
    turrets::processTurrets(mobTurrets, shells, particles, presets, worldSounds, camera);
    particles::updateParticles(particles);
    // Build when spans are used and can be spoiled.
    construction::buildBlueprints(mobs.getSoa(), presets, world->getBlueprints(), builtInScripts);
    // Clean up only after all processing.
    shells::cleanupShells(shells, presets);
    mobs::cleanupMobs(mobs, presets);
    blocks.getMeta().cleanUp();
    timeCount.update();
    builtInScripts.execute(engine, timeCount);
}

void GameSession::update(Engine& engine, const Presets& presets, const ScriptsHandler& scriptsHandler) {
    Events::reset(); // for editor // needs update
    auto& mainWindow = engine.getMainWindow();
    auto& renderer = mainWindow.getRenderer();
    auto& shaders = engine.getAssets().getShaders();
    //
    mainWindow.pollEvents();
    gui->callback();
    const bool paused = pausedManually || gui->overlapsWorld() || (Settings::gameplay.pauseInBackground && !mainWindow.isFocused());
    if (!gui->overlapsWorld())
        playerController.update(mainWindow.getInput(), camera, paused, world->getMobs().getSoa(),
            world->getBlocks().getMeta().getTurrets().getSoa(), presets);
    if (!paused) {
        for (int i = 0; i < tickSpeed; ++i) {
            updateSimulation(presets, engine);
        }
    }
    scriptsHandler.execute();
    //
    mainWindow.clear();
    camera.update(mainWindow.getSize(), mainWindow.getRealFrameDelayNs());
    renderer.setShaderProgram(*shaders.baseShader);
    renderer.setView(camera.getMapScale(), camera.getTranslation());
    worldDrawer.draw(camera, renderer, *world, presets, engine.getAssets(), timeCount.getTickCount());
    gui->drawDiegeticElements(renderer);           // temporary update will be related with blueprints
    world->getBlueprints().drawGhosts(renderer, engine);
    renderer.setShaderProgram(*shaders.emergeShader);
    world->getBlueprints().drawInProgress(renderer, engine); // temporary
    worldSounds.play(engine.getAssets().getAudio(), camera);
    //
    renderer.setShaderProgram(*shaders.shieldShader);
    mobs::drawMobShields(world->getMobs().getSoa(), presets, camera, renderer, timeCount.getTickCount());
    //
    renderer.setShaderProgram(*shaders.lightingShader);
    renderer.setView(1.f, PixelCoord(0.f, 0.f));
    drawParticles(camera, renderer, world->getParticles().getSoa());
    shells::drawShellsLighting(world->getShells().getSoa(), presets, camera, renderer);
    //
    renderer.setShaderProgram(*shaders.uiShader);
    gui->draw(renderer, engine.getAssets().getAtlas());
    mainWindow.render();

    if (mainWindow.hasLostFocus())   onLostFocus(engine);
    if (mainWindow.hasGainedFocus()) onGainedFocus(engine);
}

void GameSession::setPaused(const bool flag, Engine& engine) {
    pausedManually = flag;
    auto& audio = engine.getAssets().getAudio();
    if (pausedManually) audio.pauseWorldSounds();
    else                audio.resumeWorldSounds();
}

void GameSession::onLostFocus(Engine& engine) { 
    if (Settings::audio.muteInBackground) {
        engine.getAssets().getAudio().setMasterVolume(0.f);
        engine.getAssets().getAudio().updateVolume();
    }
}

void GameSession::onGainedFocus(Engine& engine) {
    if (Settings::audio.muteInBackground) {
        engine.getAssets().getAudio().setMasterVolume(static_cast<float>(Settings::audio.master) / 100.f);
        engine.getAssets().getAudio().updateVolume();
    }
}
