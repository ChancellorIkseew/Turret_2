#pragma once
#include <memory>
#include <vector>
#include "engine/coords/tile_coord.hpp"

class World;

struct OverlayPreset {
    uint8_t type = 0U;
    int frequency = 0, deposite = 0;
    OverlayPreset(const uint8_t type, const int frequency, const int deposite) :
        type(type), frequency(frequency), deposite(deposite) { }
};

struct WorldProperties {
    WorldProperties() = default;
    WorldProperties(const TileCoord mapSize, const uint64_t seed, const std::vector<OverlayPreset>& overlayPresets) :
        mapSize(mapSize), seed(seed), overlayPresets(overlayPresets) { }
    TileCoord mapSize = TileCoord(20, 20);
    uint64_t seed = 0U;
    std::vector<OverlayPreset> overlayPresets;
};

namespace gen {
    std::unique_ptr<World> generateWorld(const WorldProperties& properties);
}
