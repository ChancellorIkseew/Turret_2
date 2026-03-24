#include "MINGUI/widgets/ui_node.hpp"
//
#include "MINGUI/render/render_queue.hpp"

MINGUI

void Node::draw(RenderQueue& queue) {
    queue.add(position, size, palette.nonInteractive);
}

bool Node::containsMouse(const Point mousePosition) const noexcept {
    const Point start = position;
    const Point end = position + size;
    return mousePosition.x >= start.x && mousePosition.x <= end.x &&
           mousePosition.y >= start.y && mousePosition.y <= end.y;
}
