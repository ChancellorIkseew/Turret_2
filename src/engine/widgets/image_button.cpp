#include "image_button.hpp"
//
#include "engine/render/renderer.hpp"
#include "engine/window/input/input.hpp"

void ImageButton::draw(const Renderer& renderer) {
    renderer.drawFast(texture, getPosition(), getSize());
}

void ImageButton::callback(const Input& input) {
    if (action && containsMouse(input) && input.jactive(LMB))
        action();
}
