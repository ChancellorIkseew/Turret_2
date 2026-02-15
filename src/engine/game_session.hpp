#pragma once
#include <memory>
#include "engine/audio/sound_queue.hpp"
#include "game/player/camera.hpp"
#include "game/player/player_controller.hpp"
#include "game/world_drawer/world_drawer.hpp"
#include "time_count.hpp"

class Engine;
class GUI;
class ScriptsHandler;
class World;

class GameSession {
    Camera camera; // First, because needs "world->map->size".
    std::unique_ptr<World> world;
    std::unique_ptr<GUI> gui;
    PlayerController playerController;
    WorldDrawer worldDrawer;
    SoundQueue worldSounds;

    TimeCount timeCount;
    int tickSpeed = 1;
    bool paused, open = true;
public:
    GameSession(std::unique_ptr<World> world, std::unique_ptr<GUI> gui, const Assets& assets, const bool paused);
    ~GameSession();

    void update(Engine& engine, const Presets& presets, const ScriptsHandler& scriptsHandler);

    World& getWorld() { return *world; }
    GUI& getGUI() { return *gui; }
    Camera& getCamera() { return camera; }
    PlayerController& getPlayerController() { return playerController; }

    void close() { open = false; }
    void setPaused(const bool flag, Engine& engine);
    void setTickSpeed(const int ticksInFrame) { tickSpeed = ticksInFrame; }
    bool isOpen() const { return open; }
    bool isPaused() const { return paused; }
    int getTickSpeed() const { return tickSpeed; }
    const TimeCount& getTimeCount() { return timeCount; }
private:
    void prepare();
    void updateSimulation(const Presets& presets);
    t1_disable_copy_and_move(GameSession)
};
