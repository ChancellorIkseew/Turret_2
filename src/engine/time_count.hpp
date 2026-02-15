#pragma once
#include <cstdint>

class TimeCount {
    uint64_t tickCount;
    uint64_t ticksPerWave;
    uint64_t ticksToNextWave;
    uint32_t waveCount;
    bool waveJustChanged = false;
public:
    TimeCount(const uint64_t tickCount, const uint64_t ticksPerWave) :
        tickCount(tickCount),
        ticksPerWave(ticksPerWave),
        ticksToNextWave(ticksPerWave - (tickCount % ticksPerWave)),
        waveCount(static_cast<uint32_t>(tickCount / ticksPerWave)) { }
    //
    uint64_t getTickCount() const { return tickCount; }
    uint64_t getTicksToNextWave() const { return ticksToNextWave; }
    uint32_t getWaveCount() const { return waveCount; }
    bool isWaveJustChanged() const { return waveJustChanged; }
    //
    void update() {
        waveJustChanged = ticksToNextWave == 0;
        if (waveJustChanged) {
            ++waveCount;
            ticksToNextWave = ticksPerWave;
        }   
        ++tickCount;
        --ticksToNextWave;
    }
};
