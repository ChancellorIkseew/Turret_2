#include "label.hpp"
//
#include "engine/io/parser/tin_parser.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/render/text.hpp"

void Label::draw(const Renderer& renderer) {
    text::drawString(renderer, visibleText, getPosition());
}

void Label::translate(const tin::Data& translations) {
    if (!translatable)
        return;
    visibleText = translations.get<std::u32string>(utf8::to_string(originalText)).value_or(originalText);
    resizeBy(visibleText);
}

void Label::resizeBy(const std::u32string& text) {
    setSize(PixelCoord(static_cast<int>(text.length()) * 8, 20));
}
