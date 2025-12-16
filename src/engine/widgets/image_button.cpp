#include "image_button.hpp"
//
#include "engine/render/text.hpp"
#include "engine/window/input/input.hpp"

void ImageButton::callback(const Input& input) {
    if (action && containsMouse(input) && input.jactive(LMB))
        action();
}
