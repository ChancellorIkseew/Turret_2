#include <MINGUI/widgets/button.hpp>

MINGUI

void Button::draw(RenderBridge& renderBridge) {
    Clickable::draw(renderBridge);
    label.setPalette(getPalette()); // temporary. Needs better fix
    label.draw(renderBridge);
}

void Button::setPosition(const Point position) {
    Node::setPosition(position);
    centerText();
}

void Button::setText(const std::string& name) {
    label.setText(name);
    centerText();
}

void Button::centerText() {
    Point margin = (getSize() - label.getSize()) / 2;
    label.setPosition(getPosition() + margin);
}
