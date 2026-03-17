#pragma once
#include "layout.hpp"
#include "ui_defs.hpp"

START_NAMESPACE_MINGUI

class Container : public Layout {
    Align align;
    bool open = true;
public:
    Container(const Align align, const Orientation orientation) :
        Layout(orientation), align(align) { }
    ~Container() override = default;
    //
    void aplyAlignment(const PixelCoord windowSize);
    //
    bool isOpen() const { return open; }
    void close() { open = false; }
};

END_NAMESPACE_MINGUI
