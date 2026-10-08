#include "surface.hpp"
//
#include <SDL3/SDL_surface.h>

Surface::~Surface() noexcept {
    SDL_DestroySurface(surface);
}

Surface::Surface(Surface&& other) noexcept {
    surface = other.surface;
    other.surface = nullptr;
}

Surface& Surface::operator=(Surface&& other) noexcept {
    if (this != &other) {
        SDL_DestroySurface(surface);
        surface = other.surface;
        other.surface = nullptr;
    }
    return *this;
}
