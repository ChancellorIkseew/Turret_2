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
    ~Button() final = default;
    //
    void draw() final;
    void callback() final;
    void setPosition(const PixelCoord position) final;
    void setText(const std::u32string& name);
    void addCallback(std::function<void()> action) {
        this->action = action;
    }
private:
    void centerText();
};
