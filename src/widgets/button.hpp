#pragma once
#include <functional>
#include "ui_node.hpp"

class Button : public Node {
    std::function<void()> action;
    std::u32string name;
public:
    Button(const int sizeX, const int sizeY, std::u32string name) :
        Node(PixelCoord(sizeX, sizeY)), name(std::move(name)) { }
    //
    void draw() override final;
    void callback() override final;
    void addCallback(std::function<void()> action) {
        this->action = action;
    }
};
