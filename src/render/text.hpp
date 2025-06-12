#pragma once
#include <string>
#include <SDL3/SDL_render.h>

namespace text {
    void drawString(const std::u32string& text, float x, float y);
    void setRenderer(SDL_Renderer* renderer);
    void setFont(const std::string& name);
}
