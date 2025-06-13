#pragma once
#include <SDL3/SDL_rect.h>
#include <string>
#include <unordered_map>

namespace packer {
    SDL_Point arrangeRects(std::unordered_map<std::string, SDL_Rect>& atlas);
}
