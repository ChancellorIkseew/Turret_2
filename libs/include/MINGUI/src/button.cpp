#include "MINGUI/widgets/button.hpp"

MINGUI

void Button::callback(UIContext& context) {
    Clickable::callback(context);
    if (context.clicked(*this) && action)
        action();
}

void Button::draw(RenderQueue& queue) {
    Node::draw(queue);
    label.draw(queue);
}

void Button::setPosition(const Point position) {
    Node::setPosition(position);
    centerText();
}

void Button::setText(const std::u32string& name) {
    label.setText(name);
    centerText();
}

void Button::centerText() {
    Point margin = (getSize() - label.getSize()) / 2;
    label.setPosition(getPosition() + margin);
}
