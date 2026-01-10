#pragma once
#include <SDL3/SDL_rect.h>

struct Texture {
    SDL_FRect rect;
};

constexpr Texture NULL_TEXTURE = Texture(SDL_FRect{0.f, 0.f, 0.f, 0.f});

constexpr bool operator==(Texture left, Texture right) noexcept {
    return left.rect.x == right.rect.x && left.rect.y == right.rect.y &&
           left.rect.w == right.rect.w && left.rect.h == right.rect.h;
}
constexpr bool operator!=(Texture left, Texture right) noexcept {
    return !(left == right);
}
