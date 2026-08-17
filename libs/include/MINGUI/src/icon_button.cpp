#include <MINGUI/widgets/icon_button.hpp>

MINGUI

void IconButton::draw(RenderBridge& renderBridge) {
    Clickable::draw(renderBridge);
    icon.draw(renderBridge);
}
void IconButton::setPosition(const Point position) {
    Node::setPosition(position);
    const Point padding = (getSize() - icon.getSize()) / 2;
    icon.setPosition(position + padding);
}
