#include <MINGUI/core/text_edit.hpp>
//<
#include <MINGUI/core/form_validator.hpp>
#include <MINGUI/core/ui_context.hpp>
#include <MINGUI/utfcpp/utf8.h>

MINGUI

void TextEdit::update(const int frameDelayMs) {
    editingActive = false;
    if (inputTimer > 0)
        inputTimer -= frameDelayMs;
}

void TextEdit::moveCarriageToCursor(const UIContext& context, const std::string& text,
    const Point nodePosition, const float glyphWidth) {
    const size_t textLength = static_cast<size_t>(utf8::distance(text.cbegin(), text.cend()));
    const Point positionInForm = context.getMousePosition() - nodePosition;
    carPos = static_cast<size_t>(positionInForm.x / glyphWidth);
    if (carPos > textLength)
        carPos = textLength;
}

static auto getCharIter(const std::string& text, const size_t charIndex) {
    auto it = text.cbegin();
    utf8::advance(it, charIndex, text.cend());
    return it;
};

static void editForm(const UIContext& context, std::string& text, const Point size,
    const std::unique_ptr<Validator>& validator, size_t& carPos, int& inputTimer, const float glyphWidth) {
    constexpr int INPUT_RELOAD = 120;
    const TextInput input = context.getTextInput();
    const size_t textLength = static_cast<size_t>(utf8::distance(text.cbegin(), text.cend()));

    if (input.arrowLeft_ && carPos > 0 && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        --carPos;
    }
    if (input.arrowRight_ && carPos < textLength && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        ++carPos;
    }
    if (input.backspace_ && carPos > 0 && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        auto itEnd = getCharIter(text, carPos);
        --carPos;
        auto itStart = getCharIter(text, carPos);
        text.erase(itStart, itEnd);
    }
    if (input.delete_ && carPos < textLength && inputTimer <= 0) {
        inputTimer = INPUT_RELOAD;
        auto itStart = getCharIter(text, carPos);
        auto itEnd = getCharIter(text, carPos + 1);
        text.erase(itStart, itEnd);
    }

    const std::string_view enteredText = input.textEntered;

    if (enteredText.empty())
        return;
    std::string validBytes;
    size_t insertedCharsCount = 0;
    size_t currentTextLength = textLength;
    auto it = enteredText.cbegin();
    auto end = enteredText.cend();

    while (it != end) {
        const auto startCodepointIt = it;
        const char32_t codepoint = utf8::next(it, end);
        const bool canInsert = static_cast<float>(currentTextLength + 1) * glyphWidth < size.x;
        if (canInsert && (!validator || validator->isValid(codepoint))) {
            validBytes.append(startCodepointIt, it);
            ++insertedCharsCount;
            ++currentTextLength;
        }
    }

    if (validBytes.empty())
        return;
    const auto insertIt = getCharIter(text, carPos);
    const size_t bytePos = static_cast<size_t>(std::distance(text.cbegin(), insertIt));
    text.insert(bytePos, validBytes);
    carPos += insertedCharsCount;
}

void TextEdit::edit(const UIContext& context, std::string& text, const Point nodeSize,
    const Point nodePosition, const float glyphWidth, const std::unique_ptr<Validator>& validator) {
    editingActive = true;
    if (context.getMouseClicked())
        moveCarriageToCursor(context, text, nodePosition, glyphWidth);
    editForm(context, text, nodeSize, validator, carPos, inputTimer, glyphWidth);
}
