#include "form.hpp"
//
#include "engine/render/text.hpp"
#include "engine/widgets/form_editor/form_editor.hpp"
#include "engine/window/input/input.hpp"

Form::~Form() {
    FormEditor::resetTarget();
}

void Form::draw() {
    Node::draw();
    text::drawString(text, getPosition());
}

void Form::callback() {
    if (validator && state == ButtonState::checked)
        validator->validateText(text);
    Clickable::callback();
    if (containsMouse() && Input::jactive(LMB))
        FormEditor::setForm(this);
}

void Form::validate() {
    if (!validator)
        return;
    validator->validateText(text);
    validator->validateValue(text);
}
