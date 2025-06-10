#pragma once
#include <functional>
#include "ui_node.hpp"

class Button : public Node {
    std::function<void()> action;
public:
    Button(const PixelCoord size) : Node(size) { }
    //
    void callback() override final;
    void addCallback(std::function<void()> action) {
        this->action = action;
    }
};
