#pragma once
#include <cstdint>
#include "config.hpp"
#include "engine/coords/math.hpp"

using TeamID = uint8_t;
using Health = int64_t;
using AngleRad = float;
using TickCount = uint16_t;

struct Explosion {
    Health damage;
    int radius;
    float fading;
};

class Hitbox {
    PixelCoord start, end;
public:
    constexpr Hitbox(const PixelCoord start, const PixelCoord end) noexcept :
        start(start), end(end) { }
    constexpr Hitbox(const PixelCoord center, const float radius) noexcept :
        start(center - PixelCoord(radius, radius)),
        end(center + PixelCoord(radius, radius)) { }

    t1_finline_cxpr bool contains(const PixelCoord pixel) const noexcept {
        return t1::contains(start, end, pixel);
    }
    t1_finline_cxpr void move(const PixelCoord vector) noexcept {
        start = start + vector;
        end = end + vector;
    }

    t1_finline_cxpr bool intersects(const Hitbox& other) const noexcept {
        return !(end.x < other.start.x || start.x > other.end.x ||
                 end.y < other.start.y || start.y > other.end.y);
    }

    t1_finline_cxpr PixelCoord overlap(const Hitbox& other) const noexcept {
        const float overlapX = std::min(end.x, other.end.x) - std::max(start.x, other.start.x);
        const float overlapY = std::min(end.y, other.end.y) - std::max(start.y, other.start.y);
        return PixelCoord(overlapX, overlapY);
    }

    t1_finline_cxpr PixelCoord getStart() const { return start; }
    t1_finline_cxpr PixelCoord getEnd() const { return end; }
};
