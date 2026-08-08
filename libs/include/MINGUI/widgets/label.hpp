#pragma once
#include <MINGUI/widgets/ui_node.hpp>
#include <MINGUI/core/utf8.hpp>

START_NAMESPACE_MINGUI

class Label : public Node {
    std::string translationKeyText;
    std::u32string visibleText;
public:
    Label(const std::string& text) :
        translationKeyText(text), visibleText(utf8::to_u32string(text)) {
        resizeBy(visibleText);
    }
    ~Label() final = default;
    //
    void setText(const std::string& text);
    //
    void draw(RenderQueue& queue) final;
    void callback(UIContext& context) final { };
private:
    void resizeBy(const std::u32string& visibleText);
};

END_NAMESPACE_MINGUI
