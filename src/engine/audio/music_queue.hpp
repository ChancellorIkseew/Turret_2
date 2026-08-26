#pragma once
#include <cstdint>
#include <string>
#include "engine/engine_command.hpp"

class Audio;

class MusicQueue {
    uint64_t noMusicPlayingTicks = 0;
    Audio& audio;
public:
    MusicQueue(Audio& audio) : audio(audio) {}
    ~MusicQueue();
    void update(const uint32_t currentWave, const GameMode gameMode);
};
