#include "ui_node.hpp"
//
#include "window/input/input.hpp"

void Node::draw() {
    sprite.setPosition(position);
    sprite.setSize(size);
    sprite.drawFast();
}

bool Node::containsMouse() const {
    const PixelCoord mouse = Input::getMouseCoord();
    const PixelCoord end = position + size;
    return mouse.x >= position.x && mouse.x <= end.x &&
           mouse.y >= position.y && mouse.y <= end.y;
}
