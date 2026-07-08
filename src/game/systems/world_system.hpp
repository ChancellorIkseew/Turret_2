#pragma once
#include <cstdint>

class Assets;
class BuiltInScripts;
class Camera;
class Engine;
class PlayerController;
class Presets;
class Renderer;
class SoundQueue;
class World;
class WorldDrawer;

namespace world {
    void update(World& world, const Camera& camera, const Presets& presets, const uint64_t timeMs,
        const PlayerController& playerController, SoundQueue& worldSounds, BuiltInScripts& scripts);
    void draw(World& world, Renderer& renderer, WorldDrawer& drawer, const Camera& camera, const Assets& assets,
        const uint64_t tickCount, Engine& engine);
}
