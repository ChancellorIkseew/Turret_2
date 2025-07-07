#pragma once
#include <array>
#include <cmath>
#include <random>

float fade(const float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float interpolateLinear(const float pointA, const float pointB, const float deltaS) {
    return pointA + deltaS * (pointB - pointA);
}

class PerlinNoise2D {
public:
    PerlinNoise2D(uint64_t seed) : rng(seed) {
        std::uniform_real_distribution<float> dist(-1.0f, 1.0);
        for (int i = 0; i < 256; ++i) {
            float x, y;
            do {
                x = dist(rng);
                y = dist(rng);
            } while (x * x + y * y > 1.0);
            gradients[i] = { x, y };
        }
    }

    float createTile(const int x, const int y, const float generationScale) {
        const float xNorm = x / generationScale; // Norm - normalized to float value (range 0.0f-1.0f).
        const float yNorm = y / generationScale;

        const uint8_t X = static_cast<uint8_t>(xNorm);
        const uint8_t Y = static_cast<uint8_t>(yNorm);

        const float xFloatPart = xNorm - floorf(xNorm);
        const float yFloatPart = yNorm - floorf(yNorm);

        // Get vectors at all corners.
        const float n00 = dotGridGradient(X, Y, xNorm, yNorm);
        const float n01 = dotGridGradient(X, Y + 1, xNorm, yNorm);
        const float n10 = dotGridGradient(X + 1, Y, xNorm, yNorm);
        const float n11 = dotGridGradient(X + 1, Y + 1, xNorm, yNorm);

        const float x1 = interpolateLinear(n00, n10, fade(xFloatPart));
        const float x2 = interpolateLinear(n01, n11, fade(xFloatPart));
        return (interpolateLinear(x1, x2, fade(yFloatPart)) + 1.0f) / 2.0f;
    }

private:
    float dotGridGradient(const uint8_t ix, const uint8_t iy, const float x, const float y) {
        // Compute the distance vector.
        const float dx = x - static_cast<float>(ix);
        const float dy = y - static_cast<float>(iy);

        // Dot-product with the gradient vector.
        return (dx * gradients[permutations[ix + permutations[iy]]].first +
            dy * gradients[permutations[ix + permutations[iy]]].second);
    }

    std::mt19937_64 rng;
    std::pair<float, float> gradients[256];
    std::array<uint8_t, 256> permutations = {
      151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
      8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
      35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
      134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
      55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,
      169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,
      124,123,5,202,38,147,118,126,255,82,85,212,207,16,114,19,254,251,239,4,205,232,
      182,119,107,34,162,24,113,215,214,47,224,58,172,104,176,28,44,243,129,97,128,
      106,154,223,101,184,210,189,227,246,59,221,138,42,152,204,163,84,108,93,248,
      253,81,39,51,155,236,31,222,195,110,49,183,50,115,67,72,218,206,241,43,150,121,
      181,20,235,167,98,145,22,26,28,156,238,66,127,170,41,185,160,180,14,239,187,141,
      70,56,178,79,193,179,172,162,100,28,155,100,57,182,98,100,157
    };
};
