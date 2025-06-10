#include "button.hpp"
//
#include "window/input/input.hpp"

void Button::callback() {
    if (action && containsMouse() && Input::jactive(BindName::LMB))
        action();
}
