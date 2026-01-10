#include "form.hpp"
//
#include "engine/render/text.hpp"
#include "engine/widgets/form_editor/form_editor.hpp"
#include "engine/window/input/input.hpp"

void Form::draw(const Renderer& renderer) {
    Node::draw(renderer);
    text::drawString(renderer, text, getPosition());
}

void Form::callback(const Input& input) {
    if (validator && state == ButtonState::checked)
        validator->validateText(text);
    Clickable::callback(input);
    if (containsMouse(input) && input.jactive(LMB))
        FormEditor::setForm(input, weak_from_this());
}

void Form::validate() {
    if (!validator)
        return;
    validator->validateText(text);
    validator->validateValue(text);
}
