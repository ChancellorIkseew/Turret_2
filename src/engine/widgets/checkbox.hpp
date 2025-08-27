#pragma once
#include "ui_defs.hpp"
#include "ui_node.hpp"

class Checkbox : public Node {
    ButtonState state;
public:
    Checkbox(const bool flag) : state(flag ? ButtonState::checked : ButtonState::idle),
        Node(PixelCoord(20, 20), flag ? BTN_CHECKED : BTN_IDLE) { }
    ~Checkbox() final = default;
    //
    void callback() final;
    bool getValue() const { return state == ButtonState::checked; }
private:
    void setState(const ButtonState newState);
};
