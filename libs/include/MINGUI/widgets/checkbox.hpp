#pragma once
#include <MINGUI/widgets/clickable.hpp>

START_NAMESPACE_MINGUI

class Checkbox : public Clickable {
public:
    Checkbox(const bool flag) : Clickable(Point(20, 20), flag ? ButtonState::checked : ButtonState::idle) { }
    ~Checkbox() final = default;
    //
    void callback(UIContext& context) final;
    bool getValue() const { return state == ButtonState::checked; }
};

END_NAMESPACE_MINGUI
