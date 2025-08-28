#pragma once
#include "layout.hpp"

class Selector : public Layout {
public:
    Selector(const Orientation orientation) : Layout(orientation) { }
    ~Selector() final = default;
    //
    void callback() final;
};
