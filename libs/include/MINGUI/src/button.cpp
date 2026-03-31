#include <MINGUI/widgets/button.hpp>

MINGUI

void Button::draw(RenderQueue& queue) {
    Clickable::draw(queue);
    label.draw(queue);
}

void Button::setPosition(const Point position) {
    Node::setPosition(position);
    centerText();
}

void Button::setText(const std::string& name) {
    label.setText(name);
    centerText();
}

void Button::translate(const Localization& localization) {
    label.translate(localization);
    centerText();
}

void Button::centerText() {
    Point margin = (getSize() - label.getSize()) / 2;
    label.setPosition(getPosition() + margin);
}
