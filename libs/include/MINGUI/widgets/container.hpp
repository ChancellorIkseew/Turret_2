#pragma once
#include <MINGUI/widgets/layout.hpp>

START_NAMESPACE_MINGUI

class Container : public Layout {
    Align align;
    bool open = true;
    bool dirty = true;
public:
    Container(const Align align, const Orientation orientation) :
        Layout(orientation), align(align) { }
    ~Container() override = default;
    //
    void applyAlignment(const Point windowSize);
    //
    bool isOpen() const noexcept { return open; }
    bool isDirty() const noexcept { return dirty; }
    void close() noexcept { open = false; }
    void markDirty(const bool flag = true) noexcept { dirty = flag; }
};

END_NAMESPACE_MINGUI
