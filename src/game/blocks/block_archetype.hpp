#pragma once
#include <cstdint>
#include <string>

enum class BlockArchetype : uint8_t {
    air,
    wall,
    drill,
    factory,
    turret
};

namespace block_archetype {
    BlockArchetype getBlockArchetype(const std::string& name);
}

using Item = uint8_t;
using ItemCount = int64_t;
