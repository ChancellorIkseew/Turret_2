#include "selector.hpp"
//
#include "button.hpp"
#include "engine/window/input/input.hpp"

void Selector::callback() {
    for (auto& it : contents) {
        it->callback();
        if (it->containsMouse() && Input::jactive(LMB)) {
            if (std::dynamic_pointer_cast<Button>(it))
                setTarget(std::static_pointer_cast<Button>(it));
            return;
        }
    }
}

void Selector::setTarget(const Button* const button) {
    for (auto& it : contents) {
        if (it.get() == button)
            setTarget(std::static_pointer_cast<Button>(it));
    }
}

void Selector::setTarget(std::shared_ptr<Button> button) {
    if (target.lock())
        target.lock()->setState(ButtonState::idle);
    target = button;
    target.lock()->setState(ButtonState::checked);
}
