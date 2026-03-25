#include <MINGUI/widgets/label.hpp>
//
#include <MINGUI/core/localization.hpp>
#include <MINGUI/render/render_queue.hpp>

MINGUI

void Label::draw(RenderQueue& queue) {
    queue.add(getPosition(), visibleText, getPalette().text);
}

void Label::translate(const Localization& localization) {
    auto& key = translationKeyText;
    if (!translatable)
        visibleText = utf8::to_u32string(key);
    else
        visibleText = utf8::to_u32string(localization.get(key).value_or(key));
    resizeBy(visibleText);
}

void Label::resizeBy(const std::u32string& visibleText) {
    setSize(Point(static_cast<int>(visibleText.length()) * 8, 20));
}

void Label::setText(const std::string& text) {
    translationKeyText = text;
}
