#include "text.hpp"
//
#include <cstdint>
#include "atlas.hpp"
#include "config.hpp"

constexpr uint32_t CYRILIC_BEGIN = 1024U;
constexpr uint32_t SYMBOLS_PER_LINE = 16U;
constexpr float GLYPH_SIZE = 16.0f;
static SDL_FPoint startLatin;
static SDL_FPoint startCyrilic;
namespace text { static SDL_Renderer* renderer; }

static t1_finline void drawGlyph(uint32_t symbol, const SDL_FRect* destRect) noexcept {
    SDL_FRect glyphRect = SDL_FRect(startLatin.x, startLatin.y, GLYPH_SIZE, GLYPH_SIZE);
    if (symbol >= CYRILIC_BEGIN) {
        glyphRect.x = startCyrilic.x;
        glyphRect.y = startCyrilic.y;
        symbol -= CYRILIC_BEGIN;
    }
    glyphRect.x += static_cast<float>(symbol % SYMBOLS_PER_LINE) * GLYPH_SIZE;
    glyphRect.y += static_cast<float>(symbol / SYMBOLS_PER_LINE) * GLYPH_SIZE;
    SDL_RenderTexture(text::renderer, Atlas::rawSDL(), &glyphRect, destRect);
}

void text::drawString(const std::u32string& text, const float x, const float y) {
    SDL_FRect destRect(ceilf(x), ceilf(y), GLYPH_SIZE, GLYPH_SIZE);
    for (const auto it : text) {
        if (it != ' ')
            drawGlyph(it, &destRect);
        destRect.x += GLYPH_SIZE / 2.0f;
    }
}

void text::setFont(const std::string& latin, const std::string& cyrilic) {
    startLatin.x = Atlas::at(latin).x;
    startLatin.y = Atlas::at(latin).y;
    startCyrilic.x = Atlas::at(cyrilic).x;
    startCyrilic.y = Atlas::at(cyrilic).y;
}

void text::setRenderer(SDL_Renderer* renderer) {
    text::renderer = renderer;
}
