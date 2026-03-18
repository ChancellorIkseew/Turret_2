#include "MINGUI/widgets/form.hpp"
//
#include "MINGUI/render/ui_render.hpp"
#include "engine/window/input/input.hpp"

MINGUI

void Form::draw(RenderQueue& queue) {
    Node::draw(queue);
    queue.add(getPosition(), text);
    if (state == ButtonState::checked) {
        auto position = getPosition();
        position.x += static_cast<float>(carrigePosition) * (glyphSize.x / 2.0f);
        position.y += 1.0f;
        queue.add(position, Point(1.0f, glyphSize.y), static_cast<uint32_t>(UIColor::carrige));
    }
}

void Form::callback(UIContext& context) {
    Clickable::callback(context);
    //Input& input = context.input;
    if (state == ButtonState::checked) {
        if (context.clickedOut(*this)) {
            if (validator)
                validator->validateValue(text);
            setState(ButtonState::idle);
        }
        else {
            //carrigePosition = input.getTextEdit().getCarrigePosition();
            //input.getTextEdit().edit(input, text, getSize(), getPosition(), glyphSize.x, validator);
        }
    }
    else if (context.clicked(*this)) {
        setState(ButtonState::checked);
        //input.getTextEdit().moveCarriageToCursor(input, text, getPosition(), glyphSize.x);
    }
}
