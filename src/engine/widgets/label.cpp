#include "label.hpp"
//
#include "engine/render/text.hpp"

void Label::draw() {
    text::drawString(name, getPosition());
}
