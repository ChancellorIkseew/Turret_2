#include "MINGUI/widgets/image_button.hpp"
//
#include "engine/render/renderer.hpp"

MINGUI

void ImageButton::draw(RenderQueue& render) {
    //renderer.drawFast(texture, getPosition(), getSize());
}

void ImageButton::callback(UIContext& context) {
    if (action && context.clicked(*this))
        action();
}
