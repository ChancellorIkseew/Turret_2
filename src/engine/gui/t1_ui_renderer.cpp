#include "t1_ui_renderer.hpp"
//
#include "MINGUI/utfcpp/utf8.h"
#include "engine/coords/math.hpp"
#include "engine/render/text.hpp"

void T1_UIRenderer::drawText(mingui::Text text) {
    PixelCoord glyphPosition = t1::ceil(PixelCoord(text.position.x, text.position.y));
    auto current = text.string.begin();
    const auto end = text.string.end();

    while (current != end) {
        const char32_t symbol = utf8::next(current, end);
        //
        if (symbol == '\n') {
            glyphPosition.x = text.position.x;
            glyphPosition.y += text.glyphSize.y;
            continue;
        }
        if (symbol != ' ')
            text::drawGlyph(renderer, symbol, glyphPosition, text.color);
        glyphPosition.x += text.glyphSize.x;
    }
}
