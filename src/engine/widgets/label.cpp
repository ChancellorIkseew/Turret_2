#include "label.hpp"
//
#include "engine/parser/tin_parser.hpp"
#include "engine/parser/validator.hpp"
#include "engine/render/text.hpp"

void Label::draw() {
    text::drawString(visibleName, getPosition());
}

void Label::translate(const tin::Data& translations) {
    visibleName = translations.getU32String(validator::toStdString(name).c_str()).value_or(name);
    setSize(PixelCoord(static_cast<int>(visibleName.length()) * 8, 16));
}
