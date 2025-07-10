#pragma once
#include <cstdint>
#include "config.hpp"
#include "engine/coords/pixel_coord.hpp"

using Health = int64_t;
using LifeTime = int16_t;

struct Explosion {
    Health damage;
    int radius;
    float fading;
};

class Hitbox {
    PixelCoord start, end;
public:
    Hitbox(const PixelCoord start, const PixelCoord end) :
        start(start), end(end) { }

    t1_finline bool contains(const PixelCoord pixel) const noexcept {
        return pixel.x >= start.x && pixel.x <= end.x &&
            pixel.y >= start.y && pixel.y <= end.y;
    }
    t1_finline void move(const PixelCoord vector) noexcept {
        start = start + vector;
        end = end + vector;
    }
};
