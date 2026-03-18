#pragma once
#include "ui_node.hpp"

START_NAMESPACE_MINGUI

class Clickable : public Node {
protected:
    ButtonState state = ButtonState::idle;
public:
    Clickable(const Point size, const ButtonState state) : Node(size) { setState(state); }
    Clickable(const Point size)                          : Node(size, UIColor::idle) { }
    ~Clickable() override = default;
    //
    void callback(UIContext& context) override;
    void setState(const ButtonState state) { this->state = state; };
    const ButtonState getState() const { return state; }
};

END_NAMESPACE_MINGUI
