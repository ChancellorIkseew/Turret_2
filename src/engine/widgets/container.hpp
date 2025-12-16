#pragma once
#include <functional>
#include "layout.hpp"
#include "ui_defs.hpp"

class Container : public Layout {
    std::function<void()> passiveAction;
    Align align;
    bool open = true;
public:
    Container(const Align align, const Orientation orientation) :
        Layout(orientation), align(align) { }
    ~Container() final = default;
    //
    void callback(const Input& input) final {
        Layout::callback(input);
        if (passiveAction)
            passiveAction();
    }
    void addPassiveAction(std::function<void()> action) {
        passiveAction = action;
    }
    //
    void aplyAlignment(const PixelCoord windowSize);
    //
    bool isOpen() const { return open; }
    void close() { open = false; }
};
