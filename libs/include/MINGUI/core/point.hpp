#pragma once
#include <compare>
#include "ui_config.hpp"

START_NAMESPACE_MINGUI

struct Point {
    float x = 0.0f, y = 0.0f;

    constexpr Point(const int x, const int y) noexcept : x(static_cast<float>(x)), y(static_cast<float>(y)) {}
    constexpr Point(const float x, const float y) noexcept : x(x), y(y) {}
    constexpr Point() noexcept = default;

    constexpr auto operator<=>(const Point& rhs) const noexcept = default;

    constexpr Point operator+(const Point rhs) const noexcept {
        return { x + rhs.x, y + rhs.y };
    }
    constexpr Point operator-(const Point rhs) const noexcept {
        return { x - rhs.x, y - rhs.y };
    }
    constexpr Point operator*(const float value) const noexcept {
        return { x * value, y * value };
    }
    constexpr Point operator/(const float value) const noexcept {
        const float inv = 1.0f / value;
        return { x * inv, y * inv };
    }

    constexpr Point& operator+=(const Point rhs) noexcept {
        x += rhs.x; y += rhs.y; return *this;
    }
    constexpr Point& operator-=(const Point rhs) noexcept {
        x -= rhs.x; y -= rhs.y; return *this;
    }
    constexpr Point& operator*=(const float value) noexcept {
        x *= value; y *= value; return *this;
    }
    constexpr Point& operator/=(const float value) noexcept {
        const float inv = 1.0f / value;
        x *= inv; y *= inv; return *this;
    }
};

END_NAMESPACE_MINGUI
