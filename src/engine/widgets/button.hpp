#pragma once
#include <functional>
#include "clickable.hpp"
#include "label.hpp"

class Button : public Clickable {
    std::function<void()> action;
    Label label;
public:
    Button(const int sizeX, const int sizeY, std::u32string name) : Clickable(PixelCoord(sizeX, sizeY)), label(name) { }
    Button(const PixelCoord size, std::u32string name)            : Clickable(size), label(name) { }
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
    void centerText();
};
