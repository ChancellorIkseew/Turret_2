#pragma once
#include "ui_defs.hpp"
#include "ui_node.hpp"

class Clickable : public Node {
protected:
    ButtonState state = ButtonState::idle;
public:
    Clickable(const PixelCoord size, const ButtonState state) : Node(size) { setState(state); }
    Clickable(const PixelCoord size)                          : Node(size, BTN_IDLE) { }
    ~Clickable() override = default;
    //
    void callback() override;
    void setState(const ButtonState newState);
};
