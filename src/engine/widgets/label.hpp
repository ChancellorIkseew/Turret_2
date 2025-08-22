#pragma once
#include "ui_node.hpp"

class Label : public Node {
    std::u32string name, visibleName;
public:
    Label(std::u32string name) : name(std::move(name)),
        Node(PixelCoord(static_cast<int>(name.length()) * 8, 16)) { }
    ~Label() final = default;
    //
    void setText(const std::u32string& name) {
        this->name = name;
        setSize(PixelCoord(static_cast<int>(name.length()) * 8, 16));
    }
    void draw() final;
    void translate(const tin::Data& translations) final;
    void callback() final { };
};
