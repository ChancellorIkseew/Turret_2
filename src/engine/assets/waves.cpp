#include "waves.hpp"
//
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/jtin_parser.hpp"
#include "preset_reader.hpp"

static debug::Logger logger("waves");

void Waves::load() {
    jtin::Data data = jtin::read(io::folders::WAVES / "default.jtin");
    for (const auto& it : data) {
        bool failed = false;
        PresetReader reader(it, "default.jtin", failed);
        try {
            mobWaveData.emplace_back(
                reader.get<std::string>("mob"),
                reader.get<uint32_t>("begin"),
                reader.get<uint32_t>("spacing"),
                reader.get<uint32_t>("amount"),
                reader.get<uint32_t>("scaling")
            );
        }
        catch (const std::bad_optional_access&) {
            logger.error() << "Failed to create waves data file: " << "default.jtin";
        }
    }
}

Wave Waves::getWave(const uint32_t waveNumber) const {
    Wave wave;
    for (const auto& it : mobWaveData) {
        if (it.begin > waveNumber)
            continue;
        const uint32_t wavesFromBegin = waveNumber - it.begin;
        if (it.spacing != 0 && wavesFromBegin % it.spacing != 0)
            continue;
        const uint32_t period = it.spacing == 0 ? 1 : it.spacing;
        const uint32_t wavesFromBeginSpawned = wavesFromBegin / period;
        wave.mob.push_back(it.mob);
        wave.amount.push_back(it.amount + it.scaling * wavesFromBeginSpawned);
    }
    return wave;
}
