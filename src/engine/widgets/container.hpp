#pragma once
#include "layout.hpp"
#include "ui_defs.hpp"

class Container : public Layout {
    Align align;
    bool visible = true, open = true;
    std::unique_ptr<Container> child = nullptr;
public:
    Container(const Align align, const Orientation orientation) :
        Layout(orientation), align(align) { }
    //
    void aplyAlignment(const PixelCoord windowSize);
    void draw() final {
        if (!visible)
            return;
        Layout::draw();
        if (child)
            child->draw();
    }
    void callback() final {
        if (!visible)
            return;
        Layout::callback();
        if (!child)
            return;
        child->callback();
        if (!child->isOpen())
            child.reset();
    }
    void addChild(std::unique_ptr<Container> child) {
        this->child = std::move(child);
        if (this->child)
            this->child->arrange();
    }
    //
    bool isVisible() const { return visible; }
    void setVisible(const bool flag) { visible = flag; }
    bool isOpen() const { return open; }
    void close() { open = false; }
};
