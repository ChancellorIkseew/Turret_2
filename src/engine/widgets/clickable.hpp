#pragma once
#include "ui_defs.hpp"
#include "ui_node.hpp"

class Clickable : public Node {
protected:
    ButtonState state = ButtonState::idle;
public:
    Clickable(const PixelCoord size, const ButtonState state) : Node(size) { setState(state); }
    Clickable(const PixelCoord size)                          : Node(size, UIColor::idle) { }
    ~Clickable() override = default;
    //
    void callback(const Input& input) override;
    void setState(const ButtonState newState);
    const ButtonState getState() const { return state; }
};
