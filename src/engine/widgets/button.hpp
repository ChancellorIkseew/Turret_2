#pragma once
#include <functional>
#include "clickable.hpp"
#include "label.hpp"

class Button : public Clickable {
    std::function<void()> action;
    Label label;
public:
    Button(const PixelCoord size, const std::u32string& name, const bool translateble = true)
        : Clickable(size), label(name, translateble) { }
    ~Button() final = default;
    //
    void draw() final;
    void callback(const Input& input) final;
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
