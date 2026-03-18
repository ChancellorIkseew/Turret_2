#include "MINGUI/widgets/label.hpp"
//
#include "engine/io/parser/tin_parser.hpp"
#include "engine/io/parser/validator.hpp"
#include "MINGUI/render/render_queue.hpp"

MINGUI

void Label::draw(RenderQueue& queue) {
    queue.add(getPosition(), visibleText);
}

void Label::translate(const tin::Data& translations) {
    if (!translatable)
        return;
    visibleText = translations.get<std::u32string>(utf8::to_string(originalText)).value_or(originalText);
    resizeBy(visibleText);
}

void Label::resizeBy(const std::u32string& text) {
    setSize(Point(static_cast<int>(text.length()) * 8, 20));
}
