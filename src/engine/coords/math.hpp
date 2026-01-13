#pragma once
#include <cmath>
#include "config.hpp"
#include "pixel_coord.hpp"
#include "tile_coord.hpp"

namespace t1 {
    constexpr float PI_F = 3.1415927f;
    constexpr double PI_D = 3.1415926535897931;

    t1_finline_cxpr float radToDegree(const float radian) noexcept {
        return 180.0f - radian * (180.0f / PI_F);
    }
    t1_finline_cxpr double radToDegree(const double radian) noexcept {
        return 180.0f - radian * (180.0f / PI_D);
    }

    t1_finline_cxpr int pow2i(const int value) noexcept {
        return value * value;
    }
    t1_finline_cxpr float pow2f(const float value) noexcept {
        return value * value;
    }
    t1_finline_cxpr double pow2d(const double value) noexcept {
        return value * value;
    }

    t1_finline PixelCoord ceil(const PixelCoord value) noexcept {
        return PixelCoord(std::ceil(value.x), std::ceil(value.y));
    }
    t1_finline PixelCoord floor(const PixelCoord value) noexcept {
        return PixelCoord(std::floor(value.x), std::floor(value.y));
    }

    t1_finline float atan(const PixelCoord value) noexcept {
        return std::atan2(value.x, value.y);
    }

    /*no inline*/ int ceilUpPow2(const int value) noexcept;

    t1_finline bool areCloserRect(const PixelCoord first, const PixelCoord second, const float distance) noexcept {
        return std::abs(first.x - second.x) < distance && std::abs(first.y - second.y) < distance; // abs is not cxpr
    }
    t1_finline_cxpr bool areCloserCircle(const PixelCoord first, const PixelCoord second, const float distance) noexcept {
        return pow2f(distance) > pow2f(first.x - second.x) + pow2f(first.y - second.y);
    }

    t1_finline_cxpr bool contains(const TileCoord start, const TileCoord end, const TileCoord target) noexcept {
        return target.x >= start.x && target.x <= end.x &&
               target.y >= start.y && target.y <= end.y;
    }
    t1_finline_cxpr bool contains(const PixelCoord start, const PixelCoord end, const PixelCoord target) noexcept {
        return target.x >= start.x && target.x <= end.x &&
               target.y >= start.y && target.y <= end.y;
    }
}
