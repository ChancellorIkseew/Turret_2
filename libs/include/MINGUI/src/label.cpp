#include <MINGUI/widgets/label.hpp>
//
#include <MINGUI/render/render_queue.hpp>

MINGUI

void Label::draw(RenderQueue& queue) {
    queue.add(getPosition(), glyphSize, text, getPalette().text);
}

void Label::setText(const std::string& text) {
    this->text = text;
    //TODO: utf
    setSize(Point(static_cast<float>(text.length()) * glyphSize.x, glyphSize.y));
}
