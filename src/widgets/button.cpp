#include "button.hpp"
//
#include "render/text.hpp"
#include "window/input/input.hpp"

void Button::callback() {
    if (action && containsMouse() && Input::jactive(BindName::LMB))
        action();
}

void Button::draw() {
    Node::draw();
    text::drawString(name, getPosition().x, getPosition().y);
}
