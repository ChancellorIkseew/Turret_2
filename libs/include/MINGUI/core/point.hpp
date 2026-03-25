#pragma once
#include <compare>
#include <MINGUI/core/config.hpp>

START_NAMESPACE_MINGUI

template<class T>
concept IsPoint = requires(T point) {
    point.x;
    point.y;
    std::convertible_to<decltype(point.x), float>;
    std::convertible_to<decltype(point.y), float>;
};

struct Point {
    float x = 0.0f, y = 0.0f;

    template<IsPoint T>
    constexpr Point(const T other) : x(static_cast<float>(other.x)), y(static_cast<float>(other.y)) {}
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
