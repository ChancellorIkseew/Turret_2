#pragma once
#include "ui_node.hpp"

class Label : public Node {
    std::u32string name, visibleName;
    const bool translatable;
public:
    Label(const std::u32string& name, const bool translatable = true) :
        name(name), translatable(translatable) {
        resizeBy(name);
    }
    ~Label() final = default;
    //
    void setText(const std::u32string& name) {
        this->name = name;
        visibleName = name;
        resizeBy(visibleName);
    }
    void resizeBy(const std::u32string& text);
    void draw() final;
    void translate(const tin::Data& translations) final;
    void callback() final { };
};
