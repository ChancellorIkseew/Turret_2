#include "MINGUI/widgets/icon.hpp"
//
#include "MINGUI/render/render_queue.hpp"

MINGUI

void Icon::draw(RenderQueue& queue) {
    queue.add(getPosition(), getSize(), texture.get());
}
