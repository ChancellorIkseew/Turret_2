#include "sound_queue.hpp"
//
#include <algorithm>
#include <cmath>
#include "audio.hpp"

void SoundQueue::pushSound(const std::string& name, const PixelCoord position) {
    oneShots.emplace_back(name, position);
}

void SoundQueue::pushSoundLoop(const std::string& name, const PixelCoord position) {
    auto& agg = loops[name];
    agg.accumulatedPosition = agg.accumulatedPosition + position;
    agg.count += 1;
}

void SoundQueue::play(Audio& audio, const Camera& camera, const int64_t timeMs) {
    for (const auto& [name, agg] : loops) {
        if (agg.count == 0)
            continue;
        const PixelCoord averagePos = agg.accumulatedPosition / static_cast<float>(agg.count);
        const float volumeFactor = std::min(std::sqrt(static_cast<float>(agg.count)), 2.0f);
        audio.playLoopDiegetic(name, averagePos, camera, volumeFactor, timeMs);
    }

    constexpr float PITCH_JITTER_RANGE = 0.03f; // +-3% pitch
    constexpr float VOLUME_JITTER_RANGE = 0.02f; // +-2% volume

    for (const auto& [name, position] : oneShots) {
        const float pitchOffset = jitterDist(rngEngine) * PITCH_JITTER_RANGE;
        const float volumeOffset = jitterDist(rngEngine) * VOLUME_JITTER_RANGE;
        const float finalPitch = 1.0f + pitchOffset;
        const float finalGain = 1.0f + volumeOffset;
        audio.playDiegetic(name, position, camera, finalGain, finalPitch);
    }

    audio.endFrame();
    oneShots.clear();
    loops.clear();
}
