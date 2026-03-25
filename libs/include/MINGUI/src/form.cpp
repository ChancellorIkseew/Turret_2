#include <MINGUI/widgets/form.hpp>
//
#include <MINGUI/core/text_edit.hpp>
#include <MINGUI/render/render_queue.hpp>

MINGUI

void Form::draw(RenderQueue& queue) {
    Clickable::draw(queue);
    queue.add(getPosition(), text, getPalette().text);
    if (state == ButtonState::checked) {
        auto position = getPosition();
        position.x += static_cast<float>(carrigePosition) * (glyphSize.x / 2.0f);
        position.y += 1.0f;
        queue.add(position, Point(1.0f, glyphSize.y), getPalette().text);
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
