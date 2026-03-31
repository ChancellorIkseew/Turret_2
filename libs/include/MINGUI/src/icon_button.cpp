#include <MINGUI/widgets/icon_button.hpp>

MINGUI

void IconButton::draw(RenderQueue& queue) {
    Clickable::draw(queue);
    icon.draw(queue);
}
void IconButton::setPosition(const Point position) {
    Node::setPosition(position);
    const Point padding = (getSize() - icon.getSize()) / 2;
    icon.setPosition(position + padding);
}
