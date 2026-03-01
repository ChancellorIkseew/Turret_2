#pragma once
#include <cstdint>

///@brief Implenents redused SquirrelNoise5.
class SquirellNoise2D {
    static constexpr uint32_t MAX_UINT32 = std::numeric_limits<uint32_t>::max();
    static constexpr uint32_t DEFAULT_TILE_COUNT = 10000U; // Map 100x100 tiles.
    const uint32_t seed;
public:
    constexpr SquirellNoise2D(const uint64_t seed)
        : seed(static_cast<uint32_t>(seed ^ (seed >> 32))) { }

    constexpr uint32_t createTile(const int x, const int y, const uint32_t offset) const {
        return hash(x, y, seed + offset);
    }

    constexpr bool createTile(const int x, const int y, const uint32_t frequency, const uint32_t offset) const {
        const uint32_t threshold = static_cast<uint32_t>((static_cast<uint64_t>(frequency) * MAX_UINT32) / DEFAULT_TILE_COUNT);
        return hash(x, y, seed + offset) < threshold;
    }
private:
    static constexpr uint32_t hash(const int x, const int y, const uint32_t seed) {
        constexpr uint32_t SQ5_BIT_NOISE1 = 0xd2a80a3f;
        constexpr uint32_t SQ5_BIT_NOISE2 = 0xa884f197;
        constexpr uint32_t SQ5_BIT_NOISE3 = 0x6C736F4B;

        uint32_t mangled = static_cast<uint32_t>(x);
        mangled *= SQ5_BIT_NOISE1;
        mangled += static_cast<uint32_t>(y);
        mangled ^= (mangled >> 9);
        mangled += seed;
        mangled ^= (mangled >> 11);
        mangled *= SQ5_BIT_NOISE2;
        mangled ^= (mangled >> 15);
        mangled *= SQ5_BIT_NOISE3;
        mangled ^= (mangled >> 17);

        return mangled;
    }
};
