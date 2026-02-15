#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct Wave {
    std::vector<std::string> mob;
    std::vector<uint32_t> amount;
};

struct MobWaveData {
    std::string mob;
    uint32_t begin;
    uint32_t spacing;
    uint32_t amount;
    uint32_t scaling;
};

class Waves {
    std::vector<MobWaveData> mobWaveData;
public:
    void load();
    Wave getWave(const uint32_t waveNumber) const;
};
