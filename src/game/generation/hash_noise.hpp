#pragma once
#include <cstdint>

class HashNoise2D {
    static constexpr uint32_t DEFAULT_MAP_SQUARE = 10000U;
    const uint64_t seed;
public:
    HashNoise2D(const uint64_t seed) : seed(seed) {}

    uint32_t createTile(const int x, const int y) const {
        return hash(x, y);
    }
    bool createTile(const int x, const int y, const uint32_t frequancy) const {
        uint32_t randomValue = hash(x, y) % DEFAULT_MAP_SQUARE;
        return randomValue < frequancy;
    }

private:
    static uint32_t rotl32(const uint32_t x, const int r) {
        return (x << r) | (x >> (32 - r));
    }

    uint32_t hash(const int x, const int y) const {
        struct InputData {
            uint64_t seed;
            int x;
            int y;
        };
        InputData input(seed, x, y);
        const uint8_t* data = reinterpret_cast<uint8_t*>(&input);
        return hash(data);
    }

    uint32_t hash(const uint8_t* data) const {
        uint32_t h1 = static_cast<uint32_t>(seed);

        constexpr uint32_t c1 = 0xcc9e2d51;
        constexpr uint32_t c2 = 0x1b873593;
        constexpr uint32_t r1 = 15;
        constexpr uint32_t r2 = 13;
        constexpr uint32_t m = 5;
        constexpr uint32_t n = 0xe6546b64;

        uint32_t k1 = *reinterpret_cast<const uint32_t*>(data);
        uint32_t k2 = *reinterpret_cast<const uint32_t*>(data + 4);
        uint32_t k3 = *reinterpret_cast<const uint32_t*>(data + 8);
        uint32_t k4 = *reinterpret_cast<const uint32_t*>(data + 12);

        k1 *= c1;
        k1 = rotl32(k1, r1);
        k1 *= c2;
        h1 ^= k1;
        h1 = rotl32(h1, r2);
        h1 = h1 * m + n;

        k2 *= c1;
        k2 = rotl32(k2, r1);
        k2 *= c2;
        h1 ^= k2;
        h1 = rotl32(h1, r2);
        h1 = h1 * m + n;

        k3 *= c1;
        k3 = rotl32(k3, r1);
        k3 *= c2;
        h1 ^= k3;
        h1 = rotl32(h1, r2);
        h1 = h1 * m + n;

        k4 *= c1;
        k4 = rotl32(k4, r1);
        k4 *= c2;
        h1 ^= k4;
        h1 = rotl32(h1, r2);
        h1 = h1 * m + n;

        h1 ^= 16;
        h1 ^= h1 >> 16;
        h1 *= 0x85ebca6b;
        h1 ^= h1 >> 13;
        h1 *= 0xc2b2ae35;
        h1 ^= h1 >> 16;
        return h1;
    }
};
