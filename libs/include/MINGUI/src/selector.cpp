#include <MINGUI/widgets/selector.hpp>
//
#include <MINGUI/widgets/button.hpp>

MINGUI

void Selector::callback(UIContext& context) {
    for (auto& it : contents) {
        it->callback(context);
        if (context.clicked(*it)) {
            if (AbstractButton* abstractButton = it->asAbstarctButton())
                setTarget(abstractButton);
        }
    }
}

void Selector::setTarget(const AbstractButton* const button) {
    for (auto& it : contents) {
        if (it.get() == button)
            setTarget(std::static_pointer_cast<AbstractButton>(it));
    }
}

void Selector::setTarget(std::shared_ptr<AbstractButton> button) {
    if (target.lock())
        target.lock()->setState(ButtonState::idle);
    target = button;
    target.lock()->setState(ButtonState::checked);
}
