#pragma once

struct TextureRect {
    float x, y, w, h;
};

constexpr TextureRect NULL_TEXTURE_RECT = TextureRect{ 0.f, 0.f, 0.f, 0.f };

inline constexpr bool operator==(TextureRect left, TextureRect right) noexcept {
    return left.x == right.x && left.y == right.y &&
           left.w == right.w && left.h == right.h;
}
inline constexpr bool operator!=(TextureRect left, TextureRect right) noexcept {
    return !(left == right);
}
