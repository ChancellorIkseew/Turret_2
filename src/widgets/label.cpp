#include "label.hpp"
//
#include "render/text.hpp"

void Label::draw() {
    text::drawString(name, getPosition());
}
