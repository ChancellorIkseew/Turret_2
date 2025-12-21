#pragma once
#include <limits>

constexpr int MINIMAL_INT = std::numeric_limits<int>::lowest();

struct TileCoord {
    int x = 0, y = 0;
    
    constexpr TileCoord(const int x, const int y) noexcept : x(x), y(y) {}
    TileCoord() noexcept = default;
    
    bool valid() const { return x > MINIMAL_INT; }
    
    bool operator==(const TileCoord& rhs) const noexcept {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const TileCoord& rhs) const noexcept {
        return x != rhs.x || y != rhs.y;
    }
    
    TileCoord operator+(const TileCoord& rhs) const noexcept {
        return TileCoord(x + rhs.x, y + rhs.y);
    }
    TileCoord operator-(const TileCoord& rhs) const noexcept {
        return TileCoord(x - rhs.x, y - rhs.y);
    }
    TileCoord operator*(const int value) const noexcept {
        return TileCoord(x * value, y * value);
    }
    TileCoord operator/(const int value) const noexcept {
        return TileCoord(x / value, y / value);
    }
};

constexpr TileCoord INCORRECT_TILE_COORD(MINIMAL_INT, MINIMAL_INT);
