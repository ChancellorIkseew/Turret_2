#pragma once
#include <functional>
#include "label.hpp"
#include "ui_defs.hpp"
#include "ui_node.hpp"

class Button : public Node {
    std::function<void()> action;
    Label label;
    ButtonState state = ButtonState::idle;
public:
    Button(const int sizeX, const int sizeY, std::u32string name) :
        Node(PixelCoord(sizeX, sizeY), BTN_IDLE), label(name) { }
    Button(const PixelCoord size, std::u32string name) :
        Node(size, BTN_IDLE), label(name) { }
    ~Button() final = default;
    //
    void draw() final;
    void callback() final;
    void setPosition(const PixelCoord position) final;
    void setText(const std::u32string& name);
    void translate(const tin::Data& translations) final {
        label.translate(translations);
        centerText();
    }
    void addCallback(std::function<void()> action) {
        this->action = action;
    }
private:
    void setState(const ButtonState newState);
    void centerText();
};
