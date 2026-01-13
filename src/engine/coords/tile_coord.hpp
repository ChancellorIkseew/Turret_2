#pragma once
#include <compare>
#include "config.hpp"

struct TileCoord {
    int x = 0, y = 0;

    constexpr TileCoord(const int x, const int y) noexcept : x(x), y(y) {}
    constexpr TileCoord() noexcept = default;

    constexpr auto operator<=>(const TileCoord& rhs) const noexcept = default;

    t1_finline_cxpr TileCoord operator+(const TileCoord rhs) const noexcept {
        return { x + rhs.x, y + rhs.y };
    }
    t1_finline_cxpr TileCoord operator-(const TileCoord rhs) const noexcept {
        return { x - rhs.x, y - rhs.y };
    }
    t1_finline_cxpr TileCoord operator*(const int value) const noexcept {
        return { x * value, y * value };
    }
    t1_finline_cxpr TileCoord operator/(const int value) const noexcept {
        return { x / value, y / value };
    }

    t1_finline_cxpr TileCoord& operator+=(const TileCoord rhs) noexcept {
        x += rhs.x; y += rhs.y; return *this;
    }
    t1_finline_cxpr TileCoord& operator-=(const TileCoord rhs) noexcept {
        x -= rhs.x; y -= rhs.y; return *this;
    }
    t1_finline_cxpr TileCoord& operator*=(const int value) noexcept {
        x *= value; y *= value; return *this;
    }
    t1_finline_cxpr TileCoord& operator/=(const int value) noexcept {
        x /= value; y /= value; return *this;
    }
};
