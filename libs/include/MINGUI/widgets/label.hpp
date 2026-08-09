#pragma once
#include <string>
#include <MINGUI/widgets/ui_node.hpp>

START_NAMESPACE_MINGUI

class Label : public Node {
    std::string text;
    Point glyphSize;
public:
    Label(const std::string& text, const Point glyphSize = Point(8.0f, 16.0f)) :
        glyphSize(glyphSize) { setText(text); }
    ~Label() final = default;
    //
    void setText(const std::string& text);
    //
    void draw(RenderQueue& queue) final;
    void callback(UIContext& context) final { };
    void setGlyphSize(const Point size) { glyphSize = size; }
};

END_NAMESPACE_MINGUI
