#pragma once
#include <string>
#include "ui_node.hpp"

class Form : public Node {
    std::u32string text;
public:
    Form() : Node(PixelCoord(100, 20)) {}
    ~Form() final = default;

    void draw() final;
    void callback() final;
    std::u32string& getText() { return text; }
};
