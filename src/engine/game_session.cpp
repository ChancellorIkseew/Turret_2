#include "game_session.hpp"
//
#include "engine/gui/gui.hpp"
#include "engine/scripting/scripting.hpp"
#include "game/built_in_scripts/built_in_scripts.hpp"
#include "game/particles/particles_system.hpp"
#include "game/physics/ai_system.hpp"
#include "game/physics/mobs_system.hpp"
#include "game/physics/shells_system.hpp"
#include "game/physics/turrets_system.hpp"
#include "game/world/world.hpp"
#include "game/events/events.hpp"
#include "game/physics/turret_components.hpp"

// Constuctor and destructor in cpp are needed for forward declaraton "GUI" and "World" classes in hpp.
GameSession::GameSession(std::unique_ptr<World> world, std::unique_ptr<GUI> gui, const Assets& assets, const bool paused) :
    camera(world->getMap().getSize()), world(std::move(world)), gui(std::move(gui)), worldDrawer(assets), paused(paused),
    timeCount(0, 10800) {
    prepare(assets.getPresets());
}
GameSession::~GameSession() = default;

void GameSession::prepare(const Presets& presets) {
    Team* playerTeam = world->getTeams().addTeam(U"player");
    Team* enemyTeam  = world->getTeams().addTeam(U"enemy");
    playerController.setPlayerTeam(playerTeam);
    TeamID playerTeamID = playerTeam->getID();

    {
        const auto presetID = presets.getBlockID("iron_wall");
        const auto& preset = presets.getBlock(presetID);

        world->getBlocks().addBlock(presets, TileCoord(10, 10), preset.archetype, presetID, preset.maxHealth, playerTeamID);
        world->getBlocks().addBlock(presets, TileCoord(11, 10), preset.archetype, presetID, preset.maxHealth, playerTeamID);
        world->getBlocks().addBlock(presets, TileCoord(10, 11), preset.archetype, presetID, preset.maxHealth, playerTeamID);
        world->getBlocks().addBlock(presets, TileCoord(10, 12), preset.archetype, presetID, preset.maxHealth, playerTeamID);
    }
    
    {
        const auto presetID = presets.getBlockID("gatling_turret");
        const auto& preset = presets.getBlock(presetID);
        world->getBlocks().addBlock(presets, TileCoord(10, 13), preset.archetype, presetID, preset.maxHealth, playerTeamID);
        world->getBlocks().addBlock(presets, TileCoord(10, 14), preset.archetype, presetID, preset.maxHealth, playerTeamID);
        world->getBlocks().addBlock(presets, TileCoord(10, 15), preset.archetype, presetID, preset.maxHealth, playerTeamID);
        world->getBlocks().addBlock(presets, TileCoord(10, 16), preset.archetype, presetID, preset.maxHealth, playerTeamID);
    }
}

void GameSession::updateSimulation(const Presets& presets, Engine& engine) {
    auto& blocks    = world->getBlocks();
    auto& chunks    = world->getChunks();
    auto& mobs      = world->getMobs();
    auto& shells    = world->getShells();
    auto& particles = world->getParticles();
    auto mobTurrets   = fromMobs(mobs.getSoa());
    auto blockTurrets = fromBlocks(blocks.getTurretSoa());
    //
    chunks.update(mobs.getSoa());
    shells::processShells(shells.getSoa(), mobs.getSoa(), chunks, blocks);
    mobs::processMobs(mobs.getSoa(), chunks, blocks);
    ai::updateMovingAI(mobs.getSoa(), presets, playerController);
    ai::updateShootingAI(blockTurrets, mobs.getSoa(), blocks, presets, playerController);
    ai::updateShootingAI(mobTurrets, mobs.getSoa(), blocks, presets, playerController);
    turrets::processTurrets(blockTurrets, shells, particles, presets, worldSounds, camera);
    turrets::processTurrets(mobTurrets, shells, particles, presets, worldSounds, camera);
    particles::updateParticles(particles);
    // Clean up only after all processing.
    shells::cleanupShells(shells, presets);
    mobs::cleanupMobs(mobs, presets, playerController);
    blocks::cleanupBlocks(blocks);
    timeCount.update();
    built_in_scripts::execute(engine);
}

void GameSession::update(Engine& engine, const Presets& presets, const ScriptsHandler& scriptsHandler) {
    Events::reset(); // for editor // needs update
    auto& mainWindow = engine.getMainWindow();
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
    camera.update(mainWindow.getSize());
    mainWindow.setRenderScale(camera.getMapScale());
    mainWindow.setRenderTranslation(camera.getTranslation());
    worldDrawer.draw(camera, mainWindow.getRenderer(), *world, presets, engine.getAssets(), timeCount.getTickCount());
    worldSounds.play(engine.getAssets().getAudio(), camera);
    //
    mainWindow.setRenderScale(1.0f);
    mainWindow.setRenderTranslation(PixelCoord(0.0f, 0.0f));
    gui->draw(mainWindow.getRenderer(), engine.getAssets().getAtlas());
    mainWindow.render();
    mainWindow.clear();
}

void GameSession::setPaused(const bool flag, Engine& engine) {
    paused = flag;
    auto& audio = engine.getAssets().getAudio();
    if (paused) audio.pauseWorldSounds();
    else        audio.resumeWorldSounds();
}

