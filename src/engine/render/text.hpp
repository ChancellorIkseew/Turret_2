#pragma once
#include <SDL3/SDL_render.h>
#include <string>
#include "engine/coords/pixel_coord.hpp"

namespace text {
    void drawString(const std::u32string& text, const float x, const float y);
    static void drawString(const std::u32string& text, const PixelCoord pos) {
        drawString(text, pos.x, pos.y);
    }
    void setRenderer(SDL_Renderer* renderer);
    void setFont(const std::string& name);
}
