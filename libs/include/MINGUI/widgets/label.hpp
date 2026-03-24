#pragma once
#include "ui_node.hpp"
#include "MINGUI/core/utf8.hpp"

START_NAMESPACE_MINGUI

class Label : public Node {
    std::string translationKeyText;
    std::u32string visibleText;
    const bool translatable;
public:
    Label(const char* text, const bool translatable = true) :
        translationKeyText(text), translatable(translatable) { }
    Label(const std::string& text, const bool translatable = true) :
        translationKeyText(text), translatable(translatable) { }
    ~Label() final = default;
    //
    void setText(const std::string& text);
    void resizeBy(const std::u32string& visibleText);
    //
    void draw(RenderQueue& queue) final;
    void translate(const Localization& localization) final;
    void callback(UIContext& context) final { };
};

END_NAMESPACE_MINGUI
