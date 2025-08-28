#include "selector.hpp"
//
#include "button.hpp"
#include "engine/window/input/input.hpp"

void Selector::callback() {
    for (auto& it : contents) {
        it->callback();
        if (!it->containsMouse() || !Input::jactive(LMB))
            continue;
        for (auto& it : contents) {
            static_cast<Button*>(it.get())->setState(ButtonState::idle);
        }
        static_cast<Button*>(it.get())->setState(ButtonState::checked);
        return;
    }
}
