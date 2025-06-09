#include "ui_node.hpp"

void Node::draw() {
    sprite.setPosition(position);
    sprite.setSize(size);
    sprite.drawFast();
}
