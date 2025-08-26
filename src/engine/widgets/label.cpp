#include "label.hpp"
//
#include "engine/io/parser/tin_parser.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/render/text.hpp"

void Label::draw() {
    text::drawString(visibleName, getPosition());
}

void Label::translate(const tin::Data& translations) {
    visibleName = translations.getU32String(validator::toStdString(name).c_str()).value_or(name);
    setSize(PixelCoord(static_cast<int>(visibleName.length()) * 8, 16));
}
