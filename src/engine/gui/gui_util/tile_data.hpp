#pragma once
#include <cstdint>

enum class TileComponent : uint8_t {
    floor,
    overlay,
    block
};

struct TileData {
    TileComponent component = TileComponent::floor;
    uint16_t id = 0U;
};
