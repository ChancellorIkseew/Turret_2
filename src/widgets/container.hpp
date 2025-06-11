#pragma once
#include "layout.hpp"
#include "ui_defs.hpp"

class Container : public Layout {
    bool visible = true;
    Align align = Align::none;
public:
    Container(const Align align) : Layout(), align(align) { }
    //
    void aplyAlignment(const PixelCoord windowSize);
    void draw() override final {
        if (visible)
            Layout::draw();
    }
    void callback() override final {
        if (visible)
            Layout::callback();
    }
    //
    bool isVisible() const { return visible; }
    void setVisible(const bool flag) { visible = flag; }
};
