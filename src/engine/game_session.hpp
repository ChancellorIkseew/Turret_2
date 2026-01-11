#pragma once
#include "engine/audio/sound_queue.hpp"
#include "game/player/camera.hpp"
#include "game/player/player_controller.hpp"
#include "game/world_drawer/world_drawer.hpp"

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

    uint64_t tickCount = 0;
    int tickSpeed = 1;
    bool paused, open = true;
public:
    GameSession(std::unique_ptr<World> world, std::unique_ptr<GUI> gui, Assets& assets, const bool paused);

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
private:
    void prepare();
    void updateSimulation(const Presets& presets);
};
