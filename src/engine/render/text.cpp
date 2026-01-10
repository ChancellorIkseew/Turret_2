#include "text.hpp"
//
#include <cstdint>
#include <cmath>
#include <unordered_map>
#include "atlas.hpp"
#include "config.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "renderer.hpp"

constexpr char32_t CYRILIC_BEGIN = 1024U;
constexpr char32_t CUSTOM_BEGIN = 57344U; // Unicode private use area 57344 - 63743.
constexpr char32_t SYMBOLS_PER_LINE = 16U;
constexpr float GLYPH_SIZE = 16.0f;
static SDL_FPoint startLatin;
static SDL_FPoint startCyrilic;
static SDL_FPoint startCustom;
static std::unordered_map<std::string, char32_t> customSymbols;

static t1_finline void drawGlyph(const Renderer& renderer, char32_t symbol, const PixelCoord position) noexcept {
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

    renderer.drawFast(Texture(glyphRect), position, PixelCoord(GLYPH_SIZE, GLYPH_SIZE));
}

void text::drawString(const Renderer& renderer, const std::u32string& text, const PixelCoord position) {
    PixelCoord glyphPosition = PixelCoord(ceilf(position.x), ceilf(position.y));
    for (const auto it : text) {
        if (it != ' ')
            drawGlyph(renderer, it, glyphPosition);
        glyphPosition.x += GLYPH_SIZE / 2.0f;
    }
}

void text::setFont(const Atlas& atlas, const std::string& latin, const std::string& cyrilic, const std::string& custom) {
    startLatin.x = atlas.at(latin).rect.x;
    startLatin.y = atlas.at(latin).rect.y;
    startCyrilic.x = atlas.at(cyrilic).rect.x;
    startCyrilic.y = atlas.at(cyrilic).rect.y;
    startCustom.x = atlas.at(custom).rect.x;
    startCustom.y = atlas.at(custom).rect.y;
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
