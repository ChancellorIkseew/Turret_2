#pragma once
#include <cstdint>
#include <string>
#include "game/game_state.hpp"

class Audio;

class MusicQueue {
    uint64_t noMusicPlayingTicks = 0;
public:
    MusicQueue() = default;
    void update(Audio& audio, const uint32_t currentWave, const GameMode gameMode);
};
