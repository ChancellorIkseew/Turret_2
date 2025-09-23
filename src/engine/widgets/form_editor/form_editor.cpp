#include "form_editor.hpp" 
//
#include "engine/widgets/form.hpp"
#include "engine/window/input/input.hpp"

static Form* targetForm = nullptr;

void FormEditor::setForm(Form* form) {
    if (targetForm)
        targetForm->validate();
    if (form == targetForm) {
        resetTarget();
        return;
    }
    resetTarget();
    targetForm = form;
    form->setState(ButtonState::checked);
    Input::enableTextEnter(true);
}

void FormEditor::resetTarget() {
    if (!targetForm)
        return;
    Input::enableTextEnter(false);
    targetForm->setState(ButtonState::idle);
    targetForm = nullptr;
}

void FormEditor::editForm() {
    if (!targetForm)
        return;
    //
    auto& text = targetForm->getText();
    if (Input::jactive(Backspace))
        text = text.substr(0, text.length() - 1);
    if (Input::jactive(Delete))
        text = text.substr(0, text.length() - 1);
    //
    std::optional<uint32_t> sym = Input::getLastSymbolEntered();
    if (sym.has_value())
        text += sym.value();
}
