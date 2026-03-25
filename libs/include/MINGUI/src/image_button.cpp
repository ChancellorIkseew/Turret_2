#include "MINGUI/widgets/image_button.hpp"
//
#include "MINGUI/render/render_queue.hpp"

MINGUI

void ImageButton::draw(RenderQueue& queue) {
    queue.add(getPosition(), getSize(), texture.get());
}

void ImageButton::callback(UIContext& context) {
    if (action && context.clicked(*this))
        action();
}
