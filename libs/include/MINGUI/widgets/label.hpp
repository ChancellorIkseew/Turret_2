#pragma once
#include <MINGUI/widgets/ui_node.hpp>
#include <MINGUI/core/utf8.hpp>

START_NAMESPACE_MINGUI

class Label : public Node {
    std::u32string visibleText;
    Point glyphSize = Point(8.0f, 16.0f);
public:
    Label(const std::string& text) :
        visibleText(utf8::to_u32string(text)) { resizeByText(visibleText); }
    ~Label() final = default;
    //
    void setText(const std::string& text);
    //
    void draw(RenderQueue& queue) final;
    void callback(UIContext& context) final { };
    void setGlyphSize(const Point size) { glyphSize = size; }
private:
    void resizeByText(const std::u32string& visibleText);
};

END_NAMESPACE_MINGUI
