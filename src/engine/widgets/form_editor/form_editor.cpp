#include "form_editor.hpp" 
//
#include "engine/widgets/form.hpp"
#include "engine/window/input/input.hpp"

static std::weak_ptr<Form> targetForm;
static Sprite carriage;
static int carPos = 0;
constexpr int INPUT_RELOAD = 120;
static int inputTimer = 0;

void FormEditor::init() {
    carriage = Sprite("input_carriage", PixelCoord(1, 16), PixelCoord(0, 0));
}

void FormEditor::drawCarriage() {
    if (!targetForm.lock())
        return;
    auto p = targetForm.lock()->getPosition();
    p.x += carPos * 8;
    carriage.setPosition(p);
    carriage.drawFast();
}

void FormEditor::setForm(std::weak_ptr<Form> form) {
    if (form.lock() == targetForm.lock())
        return;
    resetTarget();
    targetForm = form;
    form.lock()->setState(ButtonState::checked);
}

void FormEditor::resetTarget() {
    if (!targetForm.lock())
        return;
    targetForm.lock()->validate();
    targetForm.lock()->setState(ButtonState::idle);
    targetForm.reset();
    carPos = 0;
}

void FormEditor::editForm(const int frameDelay) {
    if (inputTimer > 0)
        inputTimer -= frameDelay;
    //
    auto& text = targetForm.lock()->getText();
    //
    if (Input::active(Arrow_left) && carPos > 0 && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        --carPos;
    }
    if (Input::active(Arrow_right) && carPos < text.length() && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        ++carPos;
    }
    if (Input::active(Backspace) && carPos > 0 && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        --carPos;
        text.erase(carPos, 1);
    }
    if (Input::active(Delete) && carPos < text.length() && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        text.erase(carPos, 1);
    }
    //
    std::optional<uint32_t> sym = Input::getLastSymbolEntered();
    if (sym.has_value() && targetForm.lock()->accepts(sym.value())) {
        text.insert(carPos, 1, sym.value());
        ++carPos;
    }
}

void FormEditor::enableInput() {
    if (targetForm.lock() && !Input::isTextEnterEnabled())
        Input::enableTextEnter(true);
    if (!targetForm.lock() && Input::isTextEnterEnabled())
        Input::enableTextEnter(false);
}

void FormEditor::update(const int frameDelay) {
    enableInput();
    if (!targetForm.lock())
        return;
    if (Input::jactive(LMB) && !targetForm.lock()->containsMouse())
        return resetTarget();
    editForm(frameDelay);
}
