#include "text.hpp"
//
#include <cstdint>
#include "atlas.hpp"
#include "config.hpp"
#include "engine/coords/math.hpp"
#include "renderer.hpp"
#include "texture_rect.hpp"

constexpr char32_t CYRILIC_BEGIN = 1024U;
constexpr char32_t SYMBOLS_PER_LINE = 16U;
constexpr float GLYPH_SIZE = 16.0f;
static SDL_FPoint startLatin;
static SDL_FPoint startCyrilic;
static PixelCoord atlasSize;

static t1_finline void drawGlyph(Renderer& renderer, char32_t symbol, const PixelCoord position) noexcept {
    TextureRect glyphRect = TextureRect(startLatin.x, startLatin.y, GLYPH_SIZE / atlasSize.x, GLYPH_SIZE / atlasSize.y);
    if (symbol >= CYRILIC_BEGIN) {
        glyphRect.x = startCyrilic.x;
        glyphRect.y = startCyrilic.y;
        symbol -= CYRILIC_BEGIN;
    }
    glyphRect.x += static_cast<float>(symbol % SYMBOLS_PER_LINE) * GLYPH_SIZE / atlasSize.x;
    glyphRect.y += static_cast<float>(symbol / SYMBOLS_PER_LINE) * GLYPH_SIZE / atlasSize.y;

    renderer.draw(glyphRect, position, PixelCoord(GLYPH_SIZE, GLYPH_SIZE), PixelCoord(0.f, 0.f), 0.f, 0xFF'FF'FF'FF);
}

void text::drawString(Renderer& renderer, const std::u32string_view text, const PixelCoord position) {
    PixelCoord glyphPosition = PixelCoord(t1::ceil(position));
    for (const auto it : text) {
        if (it != ' ')
            drawGlyph(renderer, it, glyphPosition);
        glyphPosition.x += GLYPH_SIZE / 2.0f;
    }
}

void text::setFont(const Atlas& atlas, const std::string& latin, const std::string& cyrilic) {
    startLatin.x = atlas.at(latin).x;
    startLatin.y = atlas.at(latin).y;
    startCyrilic.x = atlas.at(cyrilic).x;
    startCyrilic.y = atlas.at(cyrilic).y;
    atlasSize = atlas.getSize();
}
