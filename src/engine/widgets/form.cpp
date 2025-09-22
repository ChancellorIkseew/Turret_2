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
        validator->validate(text);
    Clickable::callback();
    if (containsMouse() && Input::jactive(LMB))
        FormEditor::setForm(this);
}
