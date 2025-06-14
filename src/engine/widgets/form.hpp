#pragma once
#include <string>
#include "ui_node.hpp"

class Form : public Node {
    std::u32string text;
public:
    Form() : Node(PixelCoord(100, 20)) {}

    void draw() override final;
    void callback() override final;
    std::u32string& getText() { return text; }
};
