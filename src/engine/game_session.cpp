#include "game_session.hpp"
//
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/scripting/scripting.hpp"
#include "game/particles/particles_system.hpp"
#include "game/systems/ai_system.hpp"
#include "game/systems/mobs_system.hpp"
#include "game/systems/shells_system.hpp"
#include "game/systems/turrets_system.hpp"
#include "game/systems/turret_components.hpp"
#include "game/world/world.hpp"
#include "game/events/events.hpp"

// Constuctor and destructor in cpp are needed for forward declaraton "GUI" and "World" classes in hpp.
GameSession::GameSession(std::unique_ptr<World> world, std::unique_ptr<GUI> gui, const Assets& assets, const bool paused) :
    camera(world->getMap().getSize()), world(std::move(world)), gui(std::move(gui)), worldDrawer(assets), paused(paused),
    timeCount(0, 10800), builtInScripts(assets, *this->world) {
    prepare(assets.getPresets());
}
GameSession::~GameSession() = default;

void GameSession::prepare(const Presets& presets) {
    Team* playerTeam = world->getTeams().addTeam(U"player");
    Team* enemyTeam  = world->getTeams().addTeam(U"enemy");
    playerController.setPlayerTeam(playerTeam);
    TeamID playerTeamID = playerTeam->getID();
    // temporary for alpha-test
    for (int i = 0; i < 10; ++i)
        builtInScripts.spawnMob(presets.getMobID("shuttle"), PixelCoord(1600, 1600), playerTeamID);
}

#include "game/frontend/build_tools/blueprint.hpp"
#include "game/built_in_scripts/built_in_scripts.hpp"

static inline void buildBP(MobSoA& soa, const Presets& presets, Blueprints& blueprints, BuiltInScripts& scripts, TeamID player) {
    if (blueprints.empty())
        return;
    for (size_t i = 0; i < soa.mobCount; ++i) {
        const auto& mobPreset = presets.getMob(soa.preset[i]);
        if (!mobPreset.canBuild)
            continue;
        Blueprint* bp = blueprints.getClosest(t1::tile(soa.position[i]));
        if (!bp)
            continue;
        if (t1::areCloserCircle(t1::tileCenter(bp->tile), soa.position[i], 128.f)) {
            if (bp->progress < 100)
                bp->progress += mobPreset.buildSpeed;
            else {
                scripts.placeBlock(bp->presetID, bp->tile, player, bp->rotation);
                blueprints.removeIfExists(bp->tile);
            }
            continue;
        }
    }
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
    buildBP(mobs.getSoa(), presets, world->getBlueprints(), builtInScripts, playerController.getPlayerTeam()->getID());
    ai::updateMovingAI(mobs.getSoa(), presets, playerController, world->getBlueprints());
    ai::updateShootingAI(blockTurrets, mobs.getSoa(), blocks, presets, playerController);
    ai::updateShootingAI(mobTurrets, mobs.getSoa(), blocks, presets, playerController);
    turrets::processTurrets(blockTurrets, shells, particles, presets, worldSounds, camera);
    turrets::processTurrets(mobTurrets, shells, particles, presets, worldSounds, camera);
    particles::updateParticles(particles);
    // Clean up only after all processing.
    shells::cleanupShells(shells, presets);
    mobs::cleanupMobs(mobs, presets, playerController);
    blocks.getMeta().cleanUp();
    timeCount.update();
    builtInScripts.execute(timeCount);
}

void GameSession::update(Engine& engine, const Presets& presets, const ScriptsHandler& scriptsHandler) {
    Events::reset(); // for editor // needs update
    auto& mainWindow = engine.getMainWindow();
    auto& renderer = mainWindow.getRenderer();
    auto& shaders = engine.getAssets().getShaders();
    //
    mainWindow.pollEvents();
    gui->callback();
    if (!gui->ownsMouse())
        playerController.update(mainWindow.getInput(), camera, paused, world->getMobs(), presets);
    if (!paused) {
        for (int i = 0; i < tickSpeed; ++i) {
            updateSimulation(presets, engine);
        }
    }
    scriptsHandler.execute();
    //
    mainWindow.clear();
    camera.update(mainWindow.getSize());
    renderer.setShaderProgram(*shaders.baseShader);
    renderer.setView(camera.getMapScale(), camera.getTranslation());
    worldDrawer.draw(camera, renderer, *world, presets, engine.getAssets(), timeCount.getTickCount());
    world->getBlueprints().draw(renderer, engine); // temporary
    gui->drawDiegeticElements(renderer);           // temporary update will be related with blueprints
    worldSounds.play(engine.getAssets().getAudio(), camera);
    //
    renderer.setShaderProgram(*shaders.lightingShader);
    renderer.setView(1.f, PixelCoord(0.f, 0.f));
    ParticlesDrawer particlesDrawer(engine.getAssets());
    particlesDrawer.draw(camera, renderer, world->getParticles().getSoa());
    shells::drawShellsLighting(world->getShells().getSoa(), presets, camera, renderer);
    //
    renderer.setShaderProgram(*shaders.uiShader);
    gui->draw(renderer, engine.getAssets().getAtlas());
    mainWindow.render();
}

void GameSession::setPaused(const bool flag, Engine& engine) {
    paused = flag;
    auto& audio = engine.getAssets().getAudio();
    if (paused) audio.pauseWorldSounds();
    else        audio.resumeWorldSounds();
}
