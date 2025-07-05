#pragma once
#include <typeinfo>

enum class TileComponent : uint8_t {
    floor,
    overlay,
    block
};

struct TileData {
    TileComponent component = TileComponent::floor;
    uint8_t id = 0U;
};
