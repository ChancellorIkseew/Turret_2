#include "MINGUI/widgets/label.hpp"
//
#include "MINGUI/core/localization.hpp"
#include "MINGUI/render/render_queue.hpp"

MINGUI

void Label::draw(RenderQueue& queue) {
    queue.add(getPosition(), visibleText);
}

void Label::translate(const Localization& localization) {
    if (!translatable)
        return;
    if (localization.contains(translationKeyText))
        visibleText = utf8::to_u32string(localization.at(translationKeyText));
    else
        visibleText = utf8::to_u32string(translationKeyText);
    resizeBy(visibleText);
}

void Label::resizeBy(const std::u32string& visibleText) {
    setSize(Point(static_cast<int>(visibleText.length()) * 8, 20));
}

void Label::setText(const std::string& text) {
    translationKeyText = text;
    visibleText = utf8::to_u32string(text);;
    resizeBy(visibleText);
}
void Label::setText(const std::string& text, const Localization& localization) {
    translationKeyText = text;
    translate(localization);
    resizeBy(visibleText);
}
