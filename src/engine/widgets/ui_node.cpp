#include "ui_node.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/window/input/input.hpp"

void Node::draw() {
    sprite.drawFast();
}

bool Node::containsMouse(const Input& input) const {
    const PixelCoord start = sprite.getPosition();
    const PixelCoord end = start + sprite.getSize();
    return t1::contains(start, end, input.getMouseCoord());
}
