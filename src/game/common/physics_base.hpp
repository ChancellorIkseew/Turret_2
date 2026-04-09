#pragma once
#include <algorithm>
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

class RectHitbox {
    PixelCoord start, end;
public:
    constexpr RectHitbox(const PixelCoord start, const PixelCoord end) noexcept :
        start(start), end(end) { }
    constexpr RectHitbox(const PixelCoord center, const float radius) noexcept :
        start(center - PixelCoord(radius, radius)),
        end(center + PixelCoord(radius, radius)) { }

    t1_finline_cxpr bool contains(const PixelCoord pixel) const noexcept {
        return t1::contains(start, end, pixel);
    }
    t1_finline_cxpr bool intersects(const RectHitbox& other) const noexcept {
        return !(end.x < other.start.x || start.x > other.end.x ||
                 end.y < other.start.y || start.y > other.end.y);
    }
    t1_finline_cxpr PixelCoord overlap(const RectHitbox& other) const noexcept {
        const float overlapX = std::min(end.x, other.end.x) - std::max(start.x, other.start.x);
        const float overlapY = std::min(end.y, other.end.y) - std::max(start.y, other.start.y);
        return PixelCoord(overlapX, overlapY);
    }

    t1_finline_cxpr PixelCoord getStart() const { return start; }
    t1_finline_cxpr PixelCoord getEnd() const { return end; }
};

class CircleHitbox {
    PixelCoord center;
    float radius;
public:
    constexpr CircleHitbox(const PixelCoord center, const float radius) noexcept :
        center(center), radius(radius) { }

    t1_finline_cxpr bool contains(const PixelCoord position) const noexcept {
        return t1::areCloserCircle(center, position, radius);
    }
    t1_finline_cxpr bool intersects(const CircleHitbox& other) const noexcept {
        return t1::areCloserCircle(center, other.center, radius + other.radius);
    }
    t1_finline_cxpr bool intersects(const RectHitbox& other) const noexcept {
        const float closestX = std::clamp(center.x, other.getStart().x, other.getEnd().x);
        const float closestY = std::clamp(center.y, other.getStart().y, other.getEnd().y);
        return t1::areCloserCircle(center, { closestX, closestY }, radius);
    }
    t1_finline_cxpr PixelCoord overlap(const CircleHitbox& other) const noexcept {
        return overlap(center - other.center, radius + other.radius);
    }
    t1_finline_cxpr PixelCoord overlap(const RectHitbox& other) const noexcept {
        const float closestX = std::clamp(center.x, other.getStart().x, other.getEnd().x);
        const float closestY = std::clamp(center.y, other.getStart().y, other.getEnd().y);
        return overlap(center - PixelCoord(closestX, closestY), radius);
    }
private:
    t1_finline_cxpr PixelCoord overlap(const PixelCoord delta, const float totalRadius) const noexcept {
        const float distanceSq = t1::pow2f(delta.x) + t1::pow2f(delta.y);
        if (distanceSq >= t1::pow2f(totalRadius))
            return { 0.0f, 0.0f };
        const float distance = std::sqrt(distanceSq);
        if (distance < 0.0001f)
            return { 0.0f, -totalRadius };
        return delta / distance * (totalRadius - distance);
    }
};
