#include "form_editor.hpp" 
//
#include "engine/render/renderer.hpp"
#include "engine/widgets/form.hpp"
#include "engine/window/input/input.hpp"

static std::weak_ptr<Form> targetForm;
static size_t carPos = 0;
constexpr int INPUT_RELOAD = 120;
constexpr uint32_t CARRIGE_COLOR = 0xFF'FF'FF'FF;
constexpr PixelCoord CARRIGE_SIZE(1.0f, 16.0f);
constexpr float GLYPH_WIDTH = 8;
static int inputTimer = 0;

void FormEditor::drawCarriage(const Renderer& renderer) {
    if (!targetForm.lock())
        return;
    auto position = targetForm.lock()->getPosition();
    position.x += static_cast<float>(carPos) * GLYPH_WIDTH;
    renderer.drawRect(CARRIGE_COLOR, position, CARRIGE_SIZE);
}

void FormEditor::setForm(const Input& input, std::weak_ptr<Form> form) {
    if (form.lock() == targetForm.lock())
        return;
    resetTarget();
    targetForm = form;
    form.lock()->setState(ButtonState::checked);
    moveCarriageToCursor(input);
}

void FormEditor::resetTarget() {
    if (!targetForm.lock())
        return;
    targetForm.lock()->validate();
    targetForm.lock()->setState(ButtonState::idle);
    targetForm.reset();
    carPos = 0;
}

void FormEditor::editForm(const Input& input, const int frameDelay) {
    if (inputTimer > 0)
        inputTimer -= frameDelay;
    //
    auto& text = targetForm.lock()->getText();
    //
    if (input.active(Arrow_left) && carPos > 0 && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        --carPos;
    }
    if (input.active(Arrow_right) && carPos < text.length() && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        ++carPos;
    }
    if (input.active(Backspace) && carPos > 0 && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        --carPos;
        text.erase(carPos, 1);
    }
    if (input.active(Delete) && carPos < text.length() && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        text.erase(carPos, 1);
    }
    //
    std::optional<uint32_t> sym = input.getLastSymbolEntered();

    if (targetForm.lock()->getSize().x >= static_cast<float>(targetForm.lock()->getText().size() + 1) * GLYPH_WIDTH) {
        if (sym.has_value() && targetForm.lock()->accepts(sym.value())) {
            text.insert(carPos, 1, sym.value());
            ++carPos;
        }
    }
}

void FormEditor::enableInput(Input& input) {
    if (targetForm.lock() && !input.isTextEnterEnabled())
        input.enableTextEnter(true);
    if (!targetForm.lock() && input.isTextEnterEnabled())
        input.enableTextEnter(false);
}

void FormEditor::update(Input& input, const int frameDelay) {
    enableInput(input);
    if (!targetForm.lock())
        return;
    if (input.jactive(LMB)) {
        if (targetForm.lock()->containsMouse(input))
            moveCarriageToCursor(input);
        else
            return resetTarget();
    }
    editForm(input, frameDelay);
}

void FormEditor::moveCarriageToCursor(const Input& input) {
    const auto positionInForm = input.getMouseCoord() - targetForm.lock()->getPosition();
    carPos = static_cast<size_t>((positionInForm / GLYPH_WIDTH).x);
    const size_t textLength = targetForm.lock()->getText().length();
    if (carPos > textLength)
        carPos = textLength;
}
