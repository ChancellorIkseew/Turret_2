#pragma once
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include "engine/coords/pixel_coord.hpp"

class Audio;
class Camera;

struct OneShotSoundEvent {
    std::string name;
    PixelCoord position;
};

struct LoopSoundAggregate {
    PixelCoord accumulatedPosition;
    size_t count = 0;
};

class SoundQueue {
    std::vector<OneShotSoundEvent> oneShots;
    std::unordered_map<std::string, LoopSoundAggregate> loops;
    std::mt19937 rngEngine{ std::random_device{}() };
    std::uniform_real_distribution<float> jitterDist{ -1.0f, 1.0f };
public:
    void pushSound(const std::string& name, const PixelCoord position);
    void pushSoundLoop(const std::string& name, const PixelCoord position);
    void play(Audio& audio, const Camera& camera, const int64_t timeMs);
};
