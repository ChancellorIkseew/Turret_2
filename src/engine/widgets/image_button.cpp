#include "image_button.hpp"
//
#include "engine/render/text.hpp"
#include "engine/window/input/input.hpp"

void ImageButton::callback() {
    if (action && containsMouse() && Input::jactive(LMB))
        action();
}
