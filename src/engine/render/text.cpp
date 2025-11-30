#include "text.hpp"
//
#include <cstdint>
#include <cmath>
#include <unordered_map>
#include "atlas.hpp"
#include "config.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"

constexpr char32_t CYRILIC_BEGIN = 1024U;
constexpr char32_t CUSTOM_BEGIN = 57344U; // Unicode private use area 57344 - 63743.
constexpr char32_t SYMBOLS_PER_LINE = 16U;
constexpr float GLYPH_SIZE = 16.0f;
static SDL_FPoint startLatin;
static SDL_FPoint startCyrilic;
static SDL_FPoint startCustom;
static std::unordered_map<std::string, char32_t> customSymbols;
namespace text { static SDL_Renderer* renderer; }

static t1_finline void drawGlyph(char32_t symbol, const SDL_FRect* destRect) noexcept {
    SDL_FRect glyphRect = SDL_FRect(startLatin.x, startLatin.y, GLYPH_SIZE, GLYPH_SIZE);
    if (symbol >= CYRILIC_BEGIN) {
        if (symbol >= CUSTOM_BEGIN) {
            glyphRect.x = startCustom.x;
            glyphRect.y = startCustom.y;
            symbol -= CUSTOM_BEGIN;
        }
        else {
            glyphRect.x = startCyrilic.x;
            glyphRect.y = startCyrilic.y;
            symbol -= CYRILIC_BEGIN;
        } 
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

void text::setFont(const std::string& latin, const std::string& cyrilic, const std::string& custom) {
    startLatin.x = Atlas::at(latin).x;
    startLatin.y = Atlas::at(latin).y;
    startCyrilic.x = Atlas::at(cyrilic).x;
    startCyrilic.y = Atlas::at(cyrilic).y;
    startCustom.x = Atlas::at(custom).x;
    startCustom.y = Atlas::at(custom).y;
}

void text::setRenderer(SDL_Renderer* renderer) {
    text::renderer = renderer;
}

void text::loadCustomSymbols() {
    tin::Data data = tin::read(io::folders::FONTS / "custom_symbols.tin");
    for (const auto& [symbolName, code] : data) {
        customSymbols.emplace(symbolName, validator::toUint32(code).value_or(U'?'));
    }
}

char32_t text::getCustomSymbol(const std::string& symbolName) {
    if (!customSymbols.contains(symbolName))
        return U'?';
    return customSymbols[symbolName];
}
