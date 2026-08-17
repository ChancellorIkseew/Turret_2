#include <MINGUI/widgets/label.hpp>
//
#include <MINGUI/render/render_bridge.hpp>
#include <MINGUI/utfcpp/utf8.h>

MINGUI

void Label::draw(RenderBridge& renderBridge) {
    renderBridge.drawText(Text(getPosition(), glyphSize, text, getPalette().text));
}

void Label::setText(const std::string& text) {
    this->text = text;
    const auto length = utf8::distance(this->text.cbegin(), this->text.cend());
    setSize(Point(static_cast<float>(length) * glyphSize.x, glyphSize.y));
}
