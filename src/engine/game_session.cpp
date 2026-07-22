#include "game_session.hpp"
//
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/scripting/scripting.hpp"
#include "engine/settings/settings.hpp"
#include "game/systems/mobs_system.hpp"
#include "game/systems/world_system.hpp"
#include "game/events/events.hpp"
#include "game/world/world.hpp"

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
    camera.teleport(t1::tileCenter(corePosition));
    //
    Inventory& inventory = playerTeam->getInventory();
    inventory.resources[0] = 999;
    inventory.resources[1] = 988;
    inventory.resources[2] = 977;
    inventory.resources[3] = 966;
}

void GameSession::updateSimulation(const Presets& presets, Engine& engine) {
    const uint64_t timeMs = engine.getMainWindow().getTimeMs();
    world::update(*world, camera, presets, timeMs, playerController, worldSounds, builtInScripts);
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
    world::draw(*world, renderer, worldDrawer, camera, engine.getAssets(), timeCount.getTickCount(), engine);
    renderer.setShaderProgram(*shaders.uiShader);
    renderer.setView(1.f, PixelCoord(0.f, 0.f));
    mobs::drawEnemyMarkers(playerController.getPlayerTeamID(), world->getMobs().getSoa(), camera, renderer);
    gui->draw(renderer, engine.getAssets().getAtlas());
    mainWindow.render();
    worldSounds.play(engine.getAssets().getAudio(), camera);
    //
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
