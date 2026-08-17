#include <MINGUI/widgets/form.hpp>
//
#include <MINGUI/core/text_edit.hpp>
#include <MINGUI/render/render_bridge.hpp>

MINGUI

void Form::draw(RenderBridge& renderBridge) {
    Clickable::draw(renderBridge);
    renderBridge.drawText(Text(getPosition(), glyphSize, text, getPalette().text));
    if (state == ButtonState::checked) {
        auto position = getPosition();
        position.x += static_cast<float>(carrigePosition) * glyphSize.x;
        position.y += 1.0f;
        renderBridge.drawRect(Rect(position, Point(1.0f, glyphSize.y), getPalette().text));
    }
}

void Form::callback(UIContext& context) {
    Clickable::callback(context);
    if (state == ButtonState::checked) {
        if (context.clickedOut(*this)) {
            if (validator)
                validator->validateValue(text);
            setState(ButtonState::idle);
        }
        else {
            carrigePosition = context.getTextEdit().getCarrigePosition();
            context.getTextEdit().edit(context, text, getSize(), getPosition(), glyphSize.x, validator);
        }
    }
    else if (context.clicked(*this)) {
        setState(ButtonState::checked);
        context.getTextEdit().moveCarriageToCursor(context, text, getPosition(), glyphSize.x);
    }
}
