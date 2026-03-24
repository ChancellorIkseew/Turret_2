#include "MINGUI/widgets/layout.hpp"

MINGUI

void Layout::applyPalette() {
    for (const auto& it : contents) {
        if (!it->getPalette().isValid())
            it->setPalette(getPalette());
        if (dynamic_cast<Layout*>(it.get()))
            static_cast<Layout*>(it.get())->applyPalette();
    }
}

void Layout::arrange() {
    if (orientation == Orientation::vertical)
        arrangeVertical();
    else
        arrangeHorizontal();
}

void Layout::arrangeVertical() {
    float maxWidth = padding * 2.0f;
    Point position = Point(padding, padding) + getPosition();
    for (const auto& it : contents) {
        it->setPosition(position);
        position.y += margin + it->getSize().y;
        if (it->getSize().x > maxWidth)
            maxWidth = it->getSize().x;
    }
    setSize(Point(maxWidth + padding * 2.0f, position.y - getPosition().y));
}

void Layout::arrangeHorizontal() {
    float maxHeight = padding * 2.0f;
    Point position = Point(padding, padding) + getPosition();
    for (const auto& it : contents) {
        it->setPosition(position);
        position.x += margin + it->getSize().x;
        if (it->getSize().y > maxHeight)
            maxHeight = it->getSize().y;
    }
    setSize(Point(position.x - getPosition().x, maxHeight + padding * 2.0f));
}

void Layout::draw(RenderQueue& queue) {
    Node::draw(queue);
    for (const auto& it : contents) {
        it->draw(queue);
    }
}
