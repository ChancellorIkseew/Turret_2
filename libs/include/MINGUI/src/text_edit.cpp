#include <MINGUI/core/text_edit.hpp>
//<
#include <MINGUI/core/form_validator.hpp>
#include <MINGUI/core/ui_context.hpp>

MINGUI

constexpr int INPUT_RELOAD = 120;

void TextEdit::update(const int frameDelay) {
    editingActive = false;
    if (inputTimer > 0)
        inputTimer -= frameDelay;
}

void TextEdit::moveCarriageToCursor(const UIContext& context, const std::u32string& text,
    const Point nodePosition, const float glyphWidth) {
    const auto positionInForm = context.getMousePosition() - nodePosition;
    carPos = static_cast<size_t>(positionInForm.x / (glyphWidth / 2.0f));
    if (carPos > text.length())
        carPos = text.length();
}

static void editForm(const UIContext& context, std::u32string& text, const Point size,
    const std::unique_ptr<mingui::Validator>& validator, size_t& carPos, int& inputTimer, const float glyphWidth) {
    const TextInput input = context.getTextInput();
    //
    if (input.arrowLeft_ && carPos > 0 && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        --carPos;
    }
    if (input.arrowRight_ && carPos < text.length() && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        ++carPos;
    }
    if (input.backspace_ && carPos > 0 && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        --carPos;
        text.erase(carPos, 1);
    }
    if (input.delete_ && carPos < text.length() && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        text.erase(carPos, 1);
    }
    //
    if (size.x >= static_cast<float>(text.size() + 1) * glyphWidth / 2.0f) {
        std::optional<uint32_t> sym = input.lastSymbolEntered;
        if (sym.has_value() && (!validator || validator && validator->isValid(sym.value()))) {
            text.insert(carPos, 1, sym.value());
            ++carPos;
        }
    }
}

void TextEdit::edit(const UIContext& context, std::u32string& text, const Point nodeSize,
    const Point nodePosition, const float glyphWidth, const std::unique_ptr<mingui::Validator>& validator) {
    editingActive = true;
    if (context.getMouseClicked())
        moveCarriageToCursor(context, text, nodePosition, glyphWidth);
    editForm(context, text, nodeSize, validator, carPos, inputTimer, glyphWidth);
}
