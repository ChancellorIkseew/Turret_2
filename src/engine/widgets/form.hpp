#pragma once
#include <string>
#include "clickable.hpp"
#include "engine/window/input/utf8/utf8.hpp"

class Form : public Clickable {
    std::u32string text;
public:
    template<typename T>
    Form(T value) : Clickable(PixelCoord(100, 20)) {
        std::string strValue = std::to_string(value);
        text = utf8::fromConstCharToU32String(strValue.c_str());
    }

    Form(std::u32string text) : Clickable(PixelCoord(100, 20)), text(text) { }
    Form()                    : Clickable(PixelCoord(100, 20)) { }
    ~Form() final;

    void draw() final;
    void callback() final;
    const std::u32string& getText() const { return text; }
    std::u32string& getText() { return text; }
};
