#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "game/world/world_config.hpp"

class Assets;
class World;

struct FloorPreset {
    std::string type;
    float height = 1.0f;
    FloorPreset(const std::string& type, const float height) :
        type(type), height(height) { }
};

struct OverlayPreset {
    std::string type;
    int frequency = 0, deposite = 0;
    OverlayPreset(const std::string& type, const int frequency, const int deposite) :
        type(type), frequency(frequency), deposite(deposite) { }
};

using FloorPresets = std::vector<FloorPreset>;
using OverlayPresets = std::vector<OverlayPreset>;

struct WorldProperties {
    WorldProperties() = default;
    WorldProperties(const WorldConfig& worldConfig,
        const FloorPresets& floorPresets,
        const OverlayPresets& overlayPresets) :
        worldConfig(worldConfig),
        floorPresets(floorPresets),
        overlayPresets(overlayPresets) { }
    FloorPresets floorPresets;
    OverlayPresets overlayPresets;
    WorldConfig worldConfig;
};

namespace gen {
    std::unique_ptr<World> generateWorld(const WorldProperties& properties, const Assets& indexes);
}
