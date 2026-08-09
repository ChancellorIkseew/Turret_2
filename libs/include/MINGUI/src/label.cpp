#include <MINGUI/widgets/label.hpp>
//
#include <MINGUI/render/render_queue.hpp>

MINGUI

void Label::draw(RenderQueue& queue) {
    queue.add(getPosition(), glyphSize, visibleText, getPalette().text);
}

void Label::resizeByText(const std::u32string& visibleText) {
    setSize(Point(static_cast<float>(visibleText.length()) * glyphSize.x, glyphSize.y));
}

void Label::setText(const std::string& text) {
    visibleText = utf8::to_u32string(text);
    resizeByText(visibleText);
}
