#include "ui_node.hpp"
//
#include "engine/render/renderer.hpp"

MINGUI

void Node::draw(const Renderer& renderer) {
    renderer.drawRect(static_cast<uint32_t>(color), position, size);
}

bool Node::containsMouse(const Point mousePosition) const noexcept {
    const Point start = position;
    const Point end = position + size;
    return mousePosition.x >= start.x && mousePosition.x <= end.x &&
           mousePosition.y >= start.y && mousePosition.y <= end.y;
}
