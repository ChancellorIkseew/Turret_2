#include "label.hpp"
//
#include "engine/io/parser/tin_parser.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/render/text.hpp"

void Label::draw() {
    text::drawString(visibleName, getPosition());
}

void Label::translate(const tin::Data& translations) {
    if (!translatable)
        return;
    visibleName = translations.getU32String(validator::toStdString(name)).value_or(name);
    resizeBy(visibleName);
}

void Label::resizeBy(const std::u32string& text) {
    setSize(PixelCoord(static_cast<int>(text.length()) * 8, 16));
}
