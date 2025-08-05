#include "layout.hpp"

void Layout::addNode(Node* node) {
    if (node)
        contents.emplace_back(node);
}

void Layout::arrange() {
    if (orientation == Orientation::vertical)
        arrangeVertical();
    else
        arrangeHorizontal();
}

void Layout::arrangeVertical() {
    float maxWidth = margin * 2.0f;
    PixelCoord position = PixelCoord(margin, margin) + getPosition();
    for (const auto& it : contents) {
        it->setPosition(position);
        position.y += margin + it->getSize().y;
        if (it->getSize().x > maxWidth)
            maxWidth = it->getSize().x;
    }
    setSize(PixelCoord(maxWidth + margin * 2.0f, position.y - getPosition().y));
}

void Layout::arrangeHorizontal() {
    float maxHeight = margin * 2.0f;
    PixelCoord position = PixelCoord(margin, margin) + getPosition();
    for (const auto& it : contents) {
        it->setPosition(position);
        position.x += margin + it->getSize().x;
        if (it->getSize().y > maxHeight)
            maxHeight = it->getSize().y;
    }
    setSize(PixelCoord(position.x - getPosition().x, maxHeight + margin * 2.0f));
}

void Layout::draw() {
    Node::draw();
    for (const auto& it : contents) {
        it->draw();
    }
}
