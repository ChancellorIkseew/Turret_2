#pragma once
#include <random>
#include <string>
#include <unordered_map>
#include "engine/coords/pixel_coord.hpp"

class Audio;
class Camera;

struct SoundAggregate {
    PixelCoord accumulatedPosition;
    size_t count = 0;
};

class SoundQueue {
    std::unordered_map<std::string, SoundAggregate> oneShots;
    std::unordered_map<std::string, SoundAggregate> loops;
    std::mt19937 rngEngine{ std::random_device{}() };
    std::uniform_real_distribution<float> jitterDist{ -1.0f, 1.0f };

public:
    void pushSound(const std::string& name, const PixelCoord position);
    void pushSoundLoop(const std::string& name, const PixelCoord position);
    void play(Audio& audio, const Camera& camera, const int64_t timeMs);
};
