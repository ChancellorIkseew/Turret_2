#pragma once
#include <cstdint>

namespace util {
    inline constexpr float randAngleRad(const uint32_t seed) {
        constexpr float PI = 3.1415926535f;
        constexpr float UINT_TO_FLOAT = 1.0f / 4294967295.0f;
        uint32_t hash = seed;
        //
        hash ^= hash >> 16;
        hash *= 0x85ebca6b;
        hash ^= hash >> 13;
        hash *= 0xc2b2ae35;
        hash ^= hash >> 16;
        //
        float normalized = static_cast<float>(hash) * UINT_TO_FLOAT;
        return normalized * PI * 2.f;
    }
}
