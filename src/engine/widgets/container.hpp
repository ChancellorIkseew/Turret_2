#pragma once
#include "layout.hpp"
#include "ui_defs.hpp"

class Container : public Layout {
    Align align;
    bool visible = true, open = true;
public:
    Container(const Align align, const Orientation orientation) :
        Layout(orientation), align(align) { }
    ~Container() final = default;
    //
    void aplyAlignment(const PixelCoord windowSize);
    void draw() final {
        if (visible)
            Layout::draw();
    }
    void callback() final {
        if (visible)
            Layout::callback();
    }
    //
    bool isVisible() const { return visible; }
    void setVisible(const bool flag) { visible = flag; }
    bool isOpen() const { return open; }
    void close() { open = false; }
};
