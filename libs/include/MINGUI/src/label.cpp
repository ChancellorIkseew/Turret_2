#include <MINGUI/widgets/label.hpp>
//
#include <MINGUI/render/render_queue.hpp>

MINGUI

void Label::draw(RenderQueue& queue) {
    queue.add(getPosition(), visibleText, getPalette().text);
}

void Label::resizeBy(const std::u32string& visibleText) {
    setSize(Point(static_cast<int>(visibleText.length()) * 8, 20));
}

void Label::setText(const std::string& text) {
    visibleText = utf8::to_u32string(text);
    resizeBy(visibleText);
}
