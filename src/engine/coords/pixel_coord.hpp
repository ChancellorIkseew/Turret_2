#pragma once
#include <limits>
#include <cstdlib>

constexpr float MINIMAL_FLOAT = std::numeric_limits<float>::lowest();

struct PixelCoord {
    float x = 0.0f, y = 0.0f;
    
    PixelCoord(const int x, const int y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}
    constexpr PixelCoord(const float x, const float y) : x(x), y(y) {}
    PixelCoord() = default;
    
    bool valid() const { return x > MINIMAL_FLOAT + epsilon; }
    
    bool operator==(const PixelCoord& rhs) const {
        return std::abs(x - rhs.x) <= epsilon && std::abs(y - rhs.y) <= epsilon;
    }
    bool operator!=(const PixelCoord& rhs) const {
        return std::abs(x - rhs.x) > epsilon || std::abs(y - rhs.y) > epsilon;
    }
    
    PixelCoord operator+(const PixelCoord& rhs) const {
        return PixelCoord(x + rhs.x, y + rhs.y);
    }
    PixelCoord operator-(const PixelCoord& rhs) const {
        return PixelCoord(x - rhs.x, y - rhs.y);
    }
    PixelCoord operator*(const float value) const {
        return PixelCoord(x * value, y * value);
    }
    PixelCoord operator/(const float value) const {
        return PixelCoord(x / value, y / value);
    }
    PixelCoord operator*(const int value) const {
        const float floatValue = static_cast<float>(value);
        return PixelCoord(x * floatValue, y * floatValue);
    }
    PixelCoord operator/(const int value) const {
        const float floatValue = static_cast<float>(value);
        return PixelCoord(x / floatValue, y / floatValue);
    }
private:
    static inline constexpr float epsilon = 0.0001f;
};

constexpr PixelCoord INCORRECT_PIXEL_COORD(MINIMAL_FLOAT, MINIMAL_FLOAT);
