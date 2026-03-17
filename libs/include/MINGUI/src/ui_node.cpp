#include "MINGUI/widgets/ui_node.hpp"
//
#include "MINGUI/render/ui_render.hpp"

MINGUI

void Node::draw(RenderQueue& queue) {
    queue.add(position, size, static_cast<uint32_t>(color));
}

bool Node::containsMouse(const Point mousePosition) const noexcept {
    const Point start = position;
    const Point end = position + size;
    return mousePosition.x >= start.x && mousePosition.x <= end.x &&
           mousePosition.y >= start.y && mousePosition.y <= end.y;
}
