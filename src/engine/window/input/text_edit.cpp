#include "text_edit.hpp"
//
#include "engine/render/renderer.hpp"
#include "engine/widgets/form.hpp"
#include "engine/widgets/form_validator/form_validator.hpp"
#include "engine/window/input/input.hpp"

constexpr int INPUT_RELOAD = 120;

void TextEdit::updateTimer(const int frameDelay) {
    if (inputTimer > 0)
        inputTimer -= frameDelay;
}

void TextEdit::moveCarriageToCursor(const Input& input, const std::u32string& text,
    const PixelCoord nodePosition, const float glyphWidth) {
    const auto positionInForm = input.getMouseCoord() - nodePosition;
    carPos = static_cast<size_t>(positionInForm.x / (glyphWidth / 2.0f));
    if (carPos > text.length())
        carPos = text.length();
}

static void editForm(const Input& input, std::u32string& text, const PixelCoord size,
    const std::unique_ptr<Validator>& validator, size_t& carPos, int& inputTimer, const float glyphWidth) {
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
    if (size.x >= static_cast<float>(text.size() + 1) * glyphWidth / 2.0f) {
        std::optional<uint32_t> sym = input.getLastSymbolEntered();
        if (sym.has_value() && (!validator || validator && validator->isValid(sym.value()))) {
            text.insert(carPos, 1, sym.value());
            ++carPos;
        }
    }
}

void TextEdit::edit(const Input& input, std::u32string& text, const PixelCoord nodeSize,
    const PixelCoord nodePosition, const float glyphWidth, const std::unique_ptr<Validator>& validator) {
    if (input.jactive(LMB))
        moveCarriageToCursor(input, text, nodePosition, glyphWidth);
    editForm(input, text, nodeSize, validator, carPos, inputTimer, glyphWidth);
}

