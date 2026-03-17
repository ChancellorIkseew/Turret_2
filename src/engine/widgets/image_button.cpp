#include "image_button.hpp"
//
#include "engine/render/renderer.hpp"

void ImageButton::draw(const Renderer& renderer) {
    renderer.drawFast(texture, getPosition(), getSize());
}

void ImageButton::callback(UIContext& context) {
    if (action && context.clicked(*this))
        action();
}
