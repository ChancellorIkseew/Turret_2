#include "ui_node.hpp"
//
#include "engine/window/input/input.hpp"

void Node::draw() {
    sprite.drawFast();
}

bool Node::containsMouse() const {
    const PixelCoord mouse = Input::getMouseCoord();
    const PixelCoord start = sprite.getPosition();
    const PixelCoord end = start + sprite.getSize();
    return mouse.x >= start.x && mouse.x <= end.x &&
           mouse.y >= start.y && mouse.y <= end.y;
}
