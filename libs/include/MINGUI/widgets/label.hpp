#pragma once
#include <string>
#include "ui_node.hpp"

START_NAMESPACE_MINGUI

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
        resizeBy(text);
    }
    void resizeBy(const std::u32string& text);
    void draw(RenderQueue& queue) final;
    void translate(const tin::Data& translations) final;
    void callback(UIContext& context) final { };
};

END_NAMESPACE_MINGUI
