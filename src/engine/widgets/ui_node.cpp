#include "ui_node.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/render/renderer.hpp"
#include "engine/window/input/input.hpp"

void Node::draw(const Renderer& renderer) {
    renderer.drawRect(static_cast<uint32_t>(color), position, size);
}

bool Node::containsMouse(const Input& input) const {
    return t1::contains(position, position + size, input.getMouseCoord());
}
