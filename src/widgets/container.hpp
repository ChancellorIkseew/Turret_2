#pragma once
#include "ui_node.hpp"
#include "layout.hpp"

class Container : public Layout {
    bool visible = true;
    Align align = Align::none;
public:
    Container(const Align align) : Layout(), align(align) { }
    //
    void draw() override final {
        if (visible)
            Layout::draw();
    }
    //
    bool isVisible() const { return visible; }
    void setVisible(const bool flag) { visible = flag; }
};
