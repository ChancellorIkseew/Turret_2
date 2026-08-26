#include "music_queue.hpp"
//
#include "audio.hpp"

MusicQueue::~MusicQueue() {
    audio.stopMusic();
}

void MusicQueue::update(const uint32_t currentWave, const GameMode gameMode) {
    const bool noMusicPlaying = !audio.isMusicPlaying();

    if (gameMode == GameMode::menu && noMusicPlaying) {
        audio.playMusic("ost_menu");
        return;
    }

    if (noMusicPlaying)
        ++noMusicPlayingTicks;
    else
        noMusicPlayingTicks = 0;

    const bool bossWave = currentWave % 5 == 0 && currentWave > 0;
    constexpr uint64_t WAIT_TICKS = 60 * 60 * 2;

    if (bossWave && noMusicPlayingTicks > WAIT_TICKS) {
        // TODO: randomize music
        audio.playMusic("ost_boss_1");
        return;
    }
    if (noMusicPlayingTicks > WAIT_TICKS) {
        // TODO: randomize music
        audio.playMusic("ost_neutral_1");
    }
}
