#include "text.hpp"
//
#include <typeinfo>
#include "atlas.hpp"

constexpr uint32_t SYMBOLS_PER_LINE = 16U;
constexpr float GLYPH_SIZE = 16.0f;
static SDL_FPoint start;
namespace text { static SDL_Renderer* renderer; }

static __forceinline void drawGlyph(const uint32_t symbol, const SDL_FRect* destRect) {
    SDL_FRect glyphRect(start.x, start.y, GLYPH_SIZE, GLYPH_SIZE);
    glyphRect.x += static_cast<float>(symbol % SYMBOLS_PER_LINE * GLYPH_SIZE);
    glyphRect.y += static_cast<float>(symbol / SYMBOLS_PER_LINE * GLYPH_SIZE);
    SDL_RenderTexture(text::renderer, Atlas::rawSDL(), &glyphRect, destRect);
}

void text::drawString(const std::u32string& text, const float x, const float y) {
    SDL_FRect destRect(x, y, GLYPH_SIZE, GLYPH_SIZE);
    for (const auto it : text) {
        if (it != ' ')
            drawGlyph(it, &destRect);
        destRect.x += GLYPH_SIZE / 2.0f;
    }
}

void text::setFont(const std::string& name) {
    start.x = Atlas::at(name).x;
    start.y = Atlas::at(name).y;
}

void text::setRenderer(SDL_Renderer* renderer) {
    text::renderer = renderer;
}

