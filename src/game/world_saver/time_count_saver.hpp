#pragma once
#include "engine/io/parser/tin_parser.hpp"
#include "game/world/time_count.hpp"

inline void saveTimeCount(const TimeCount& tickCount, const std::filesystem::path& path) {
    tin::Data data;
    data.emplace("tick_count", tickCount.getTickCount());
    data.emplace("ticks_to_next_wave", tickCount.getTicksToNextWave());
    data.emplace("wave_count", tickCount.getWaveCount());
    tin::write(path / "time_count.tin", data);
}

inline TimeCount loadTimeCount(const std::filesystem::path& path) {
    tin::Data data = tin::read(path / "time_count.tin");
    uint64_t tickCount = data.get<uint64_t>("tick_count").value();
    uint64_t ticksToNextWave = data.get<uint64_t>("ticks_to_next_wave").value();
    uint32_t waveCount = data.get<uint32_t>("wave_count").value();
    return TimeCount(tickCount, ticksToNextWave, waveCount);
}
