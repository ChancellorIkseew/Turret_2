#pragma once
#include <memory>
#include <string>
#include <vector>
#include "engine/coords/tile_coord.hpp"

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
    WorldProperties(const TileCoord mapSize,
        const uint64_t seed,
        const FloorPresets& floorPresets,
        const OverlayPresets& overlayPresets) :
        mapSize(mapSize),
        seed(seed),
        floorPresets(floorPresets),
        overlayPresets(overlayPresets) { }
    TileCoord mapSize = TileCoord(20, 20);
    uint64_t seed = 0U;
    FloorPresets floorPresets;
    OverlayPresets overlayPresets;
};

namespace gen {
    std::unique_ptr<World> generateWorld(const WorldProperties& properties);
}
