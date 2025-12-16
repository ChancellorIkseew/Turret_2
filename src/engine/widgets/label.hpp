#pragma once
#include "ui_node.hpp"

class Label : public Node {
    std::u32string originalText, visibleText;
    const bool translatable;
public:
    Label(const std::u32string& originalText, const bool translatable = true) : translatable(translatable) {
        setText(originalText);
    }
    ~Label() final = default;
    //
    void setText(const std::u32string& text) {
        originalText = text;
        visibleText = text;
        resizeBy(visibleText);
    }
    void resizeBy(const std::u32string& text);
    void draw() final;
    void translate(const tin::Data& translations) final;
    void callback(const Input& input) final { };
};
