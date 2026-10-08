#pragma once
#include <memory>
#include "engine_command.hpp"
#include "engine/audio/music_queue.hpp"
#include "engine/audio/sound_queue.hpp"
#include "game/built_in_scripts/built_in_scripts.hpp"
#include "game/player/camera.hpp"
#include "game/player/player_controller.hpp"
#include "game/world_drawer/world_drawer.hpp"
#include "time_count.hpp"

class App;
class GUI;
class ScriptsHandler;
class World;

class GameSession {
    Camera camera; // First, because needs "world->map->size".
    std::unique_ptr<World> world;
    std::unique_ptr<GUI> gui;
    PlayerController playerController;
    WorldDrawer worldDrawer;
    MusicQueue musicQueue;
    SoundQueue worldSounds;
    BuiltInScripts builtInScripts;

    TimeCount timeCount;
    std::optional<uint64_t> lastCoreAttack;
    int tickSpeed = 1;
    bool pausedManually;
    GameMode gameMode;
public:
    GameSession(std::unique_ptr<World> world, std::unique_ptr<GUI> gui, Assets& assets, const bool paused, const GameMode gameMode);
    ~GameSession();

    void update(App& app, const Presets& presets, const ScriptsHandler& scriptsHandler);

    World& getWorld() { return *world; }
    GUI& getGUI() { return *gui; }
    Camera& getCamera() { return camera; }
    PlayerController& getPlayerController() { return playerController; }

    void setPaused(const bool flag, App& app);
    void setTickSpeed(const int ticksInFrame) { tickSpeed = ticksInFrame; }
    bool isPausedManually() const { return pausedManually; }
    GameMode getGameMode() const { return gameMode; }
    int getTickSpeed() const { return tickSpeed; }
    const TimeCount& getTimeCount() const { return timeCount; }
    void startNewWave() { timeCount.startWave(); }
    BuiltInScripts& getBuiltInScripts() { return builtInScripts; }
private:
    void prepare(const Presets& presets);
    void updateSimulation(const Presets& presets, App& app);
    t1_disable_copy_and_move(GameSession)
};
