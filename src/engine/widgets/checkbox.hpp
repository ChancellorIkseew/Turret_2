#pragma once
#include "clickable.hpp"

class Checkbox : public Clickable {
public:
    Checkbox(const bool flag) : Clickable(PixelCoord(20, 20), flag ? ButtonState::checked : ButtonState::idle) { }
    ~Checkbox() final = default;
    //
    void callback(UIContext& context) final;
    bool getValue() const { return state == ButtonState::checked; }
};
