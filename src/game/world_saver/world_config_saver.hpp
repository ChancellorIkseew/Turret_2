#pragma once
#include "engine/io/parser/tin_parser.hpp"
#include "game/world/world_config.hpp"

#define key_val(value) #value, value

inline void saveConfig(const WorldConfig& c, const std::filesystem::path& path) {
    tin::Data data;
    data.emplace(key_val(c.mapSize.x));
    data.emplace(key_val(c.mapSize.y));
    data.emplace(key_val(c.seed));
    data.emplace(key_val(c.ticksPerWave));
    data.emplace(key_val(c.enemyCountMul));
    data.emplace(key_val(c.blockCostMul));
    data.emplace(key_val(c.wavesByTimer));
    tin::write(path / "config.tin", data);
}

inline WorldConfig loadConfig(const std::filesystem::path& path) {
    WorldConfig c;
    tin::Data data = tin::read(path / "config.tin");
    data.getOrThrow(key_val(c.mapSize.x));
    data.getOrThrow(key_val(c.mapSize.y));
    data.getOrThrow(key_val(c.seed));
    data.getOrThrow(key_val(c.ticksPerWave));
    data.getOrThrow(key_val(c.enemyCountMul));
    data.getOrThrow(key_val(c.blockCostMul));
    data.getOrThrow(key_val(c.wavesByTimer));
    return c;
}
