#include "form.hpp"
//
#include "engine/render/renderer.hpp"
#include "engine/render/text.hpp"
#include "engine/window/input/input.hpp"

void Form::draw(const Renderer& renderer) {
    Node::draw(renderer);
    text::drawString(renderer, text, getPosition());
    if (state == ButtonState::checked) {
        auto position = getPosition();
        position.x += static_cast<float>(carrigePosition) * (glyphSize.x / 2.0f);
        position.y += 1.0f;
        renderer.drawRect(static_cast<uint32_t>(UIColor::carrige), position, PixelCoord(1.0f, glyphSize.y));
    }
}

void Form::callback(const Input& input) {
    Clickable::callback(input);
    if (state == ButtonState::checked) {
        if (!containsMouse(input) && input.jactive(LMB)) {
            if (validator)
                validator->validateValue(text);
            setState(ButtonState::idle);
        }
        else {
            carrigePosition = input.getTextEdit().getCarrigePosition();
            input.getTextEdit().edit(input, text, getSize(), getPosition(), glyphSize.x, validator);
        }    
    }
    else {
        if (containsMouse(input) && input.jactive(LMB)) {
            setState(ButtonState::checked);
            input.getTextEdit().moveCarriageToCursor(input, text, getPosition(), glyphSize.x);
        }
    }
}
