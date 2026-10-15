#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "game/world/world_config.hpp"

class Presets;
class World;

struct FloorGenPreset {
    std::string type;
    float height = 1.0f;
    FloorGenPreset(const std::string& type, const float height) :
        type(type), height(height) { }
};

struct OreGenPreset {
    std::string type;
    int frequency = 0, deposite = 0;
    OreGenPreset(const std::string& type, const int frequency, const int deposite) :
        type(type), frequency(frequency), deposite(deposite) { }
};

using FloorGenPresets = std::vector<FloorGenPreset>;
using OreGenPresets = std::vector<OreGenPreset>;

struct WorldProperties {
    WorldProperties() = default;
    WorldProperties(const WorldConfig& worldConfig,
        const FloorGenPresets& floorGenPresets,
        const OreGenPresets& oreGenPresets) :
        worldConfig(worldConfig),
        floorGenPresets(floorGenPresets),
        oreGenPresets(oreGenPresets) { }
    FloorGenPresets floorGenPresets;
    OreGenPresets oreGenPresets;
    WorldConfig worldConfig;
};

namespace gen {
    std::unique_ptr<World> generateWorld(const WorldProperties& properties, const Presets& presets);
}
