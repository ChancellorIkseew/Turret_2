#pragma once
#include <cstdint>

class TimeCount {
    uint64_t tickCount;
    uint64_t ticksToNextWave;
    uint32_t waveCount;
    bool waveJustChanged = false;
    bool startWaveNow = false;
public:
    TimeCount(const uint64_t tickCount, const uint64_t ticksToNextWave, const uint32_t waveCount) :
        tickCount(tickCount), ticksToNextWave(ticksToNextWave), waveCount(waveCount) { }
    //
    uint64_t getTickCount() const { return tickCount; }
    uint64_t getTicksToNextWave() const { return ticksToNextWave; }
    uint32_t getWaveCount() const { return waveCount; }
    bool isWaveJustChanged() const { return waveJustChanged; }
    //
    void update(const uint64_t ticksPerWave, const bool wavesByTimer) {
        waveJustChanged = ticksToNextWave == 0 || startWaveNow;
        if (waveJustChanged) {
            ++waveCount;
            ticksToNextWave = ticksPerWave;
            startWaveNow = false;
        }   
        ++tickCount;
        if (wavesByTimer)
            --ticksToNextWave;
    }
    void startWave() {
        startWaveNow = true;
    }
};
