#pragma once
#include "layout.hpp"

START_NAMESPACE_MINGUI

class Container : public Layout {
    Align align;
    bool open = true;
public:
    Container(const Align align, const Orientation orientation) :
        Layout(orientation), align(align) { }
    ~Container() override = default;
    //
    void aplyAlignment(const Point windowSize);
    //
    bool isOpen() const noexcept { return open; }
    void close() noexcept { open = false; }
};

END_NAMESPACE_MINGUI
