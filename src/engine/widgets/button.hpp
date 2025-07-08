#pragma once
#include <functional>
#include "label.hpp"
#include "ui_node.hpp"

class Button : public Node {
    std::function<void()> action;
    Label label;
public:
    Button(const int sizeX, const int sizeY, std::u32string name) :
        Node(PixelCoord(sizeX, sizeY)), label(name) { }
    Button(const PixelCoord size, std::u32string name) :
        Node(size), label(name) { }
    //
    void setText(const std::u32string& name) {
        label.setText(name);
    }
    void draw() override final;
    void callback() override final;
    void setPosition(const PixelCoord position) override final;
    void addCallback(std::function<void()> action) {
        this->action = action;
    }
};
