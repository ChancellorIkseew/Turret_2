#pragma once
#include <functional>
#include <MINGUI/widgets/clickable.hpp>

START_NAMESPACE_MINGUI

class AbstractButton : public Clickable {
    std::function<void()> action;
public:
    AbstractButton(const Point size) : Clickable(size) {}
    ~AbstractButton() override = default;
    //
    void callback(UIContext& context) override {
        Clickable::callback(context);
        if (context.clicked(*this) && action)
            action();
    }
    void addCallback(std::function<void()> action) {
        this->action = action;
    }
    //
    AbstractButton* asAbstarctButton() noexcept final { return this; }
};

END_NAMESPACE_MINGUI
