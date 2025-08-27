#pragma once
#include "ui_defs.hpp"
#include "ui_node.hpp"

class Checkbox : public Node {
    static inline std::string BTN_IDLE = "button_idle";
    static inline std::string BTN_HOVER = "button_hover";
    static inline std::string BTN_CHECKED = "button_checked";
    ButtonState state;
public:
    Checkbox(const bool flag) : state(flag ? ButtonState::checked : ButtonState::idle),
        Node(PixelCoord(20, 20), flag ? BTN_CHECKED : BTN_IDLE) { }
    ~Checkbox() final = default;
    //
    void callback() final;
    bool getValue() { return state == ButtonState::checked; }
private:
    void setState(const ButtonState newState);
};
