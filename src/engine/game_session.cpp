#include "game_session.hpp"
//
#include "engine/gui/gui.hpp"
#include "engine/scripting/scripting.hpp"
#include "game/physics/ai_system.hpp"
#include "game/physics/mobs_system.hpp"
#include "game/physics/shells_system.hpp"
#include "game/physics/turrets_system.hpp"
#include "game/world/world.hpp"
#include "game/events/events.hpp"

// Constuctor and destructor in cpp are needed for forward declaraton "GUI" and "World" classes in hpp.
GameSession::GameSession(std::unique_ptr<World> world, std::unique_ptr<GUI> gui, const Assets& assets, const bool paused) :
    camera(world->getMap().getSize()), world(std::move(world)), gui(std::move(gui)), worldDrawer(assets), paused(paused) {
    prepare();
}
GameSession::~GameSession() = default;

void GameSession::prepare() {
    Team* playerTeam = world->getTeams().addTeam(U"player");
    Team* enemyTeam  = world->getTeams().addTeam(U"enemy");
    playerController.setPlayerTeam(playerTeam);
    world->getBlocks().placeBlock(TileCoord(10, 10), BlockPresetID(2));
}

void GameSession::updateSimulation(const Presets& presets) {
    auto& mobs = world->getMobs();
    auto& shells = world->getShells();
    //
    world->getChunks().update(mobs.getSoa());
    shells::processShells(shells.getSoa(), mobs.getSoa(), world->getChunks());
    mobs::processMobs(mobs.getSoa(), presets, world->getChunks());
    ai::updateMovingAI(mobs.getSoa(), presets, playerController);
    ai::updateShootingAI(mobs.getSoa(), presets, playerController);
    turrets::processTurrets(mobs.getSoa(), shells, presets, worldSounds, camera);
    // Clean up only after all processing.
    shells::cleanupShells(shells, presets);
    mobs::cleanupMobs(mobs, presets, playerController);
    ++tickCount;
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
            updateSimulation(presets);
        }
    }
    scriptsHandler.execute();
    //
    camera.update(mainWindow.getSize());
    mainWindow.setRenderScale(camera.getMapScale());
    mainWindow.setRenderTranslation(camera.getTranslation());
    worldDrawer.draw(camera, mainWindow.getRenderer(), *world, presets, tickCount);
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

