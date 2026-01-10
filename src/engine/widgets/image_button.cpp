#include "image_button.hpp"
//
#include "engine/render/renderer.hpp"
#include "engine/window/input/input.hpp"

void ImageButton::draw(const Renderer& renderer) {
    renderer.drawFast(texture, getPosition(), getSize());
}

void ImageButton::callback(UIContext& context) {
    if (action && containsMouse(context.input) && context.input.jactive(LMB))
        action();
}
