#include "form.hpp"
//
#include "engine/render/text.hpp"
#include "engine/widgets/form_editor/form_editor.hpp"
#include "engine/window/input/input.hpp"

void Form::draw() {
    Node::draw();
    text::drawString(text, getPosition());
}

void Form::callback() {
    bool editing = false;
    if (Input::jactive(LMB))
        FormEditor::setForm(this);
}
