#pragma once
#include "config.hpp"

struct SDL_Surface;

class Surface {
    SDL_Surface* surface = nullptr;
public:
    explicit Surface(SDL_Surface* surface) noexcept : surface(surface) {}
    ~Surface() noexcept;
    Surface(Surface&& other) noexcept;
    Surface& operator=(Surface&& other) noexcept;
    SDL_Surface* raw() const noexcept { return surface; }
private:
    t1_disable_copy(Surface)
};
