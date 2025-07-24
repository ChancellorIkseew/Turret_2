#pragma once
#include "layout.hpp"
#include "ui_defs.hpp"

class Container : public Layout {
    Align align;
    bool open = true;
public:
    Container(const Align align, const Orientation orientation) :
        Layout(orientation), align(align) { }
    ~Container() final = default;
    //
    void aplyAlignment(const PixelCoord windowSize);
    //
    bool isOpen() const { return open; }
    void close() { open = false; }
};
