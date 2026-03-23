#pragma once
#include "MINGUI/core/point.hpp"
#include <memory>
#include <string>

START_NAMESPACE_MINGUI

struct Validator;
class UIContext;

class TextEdit {
    int inputTimer = 0;
    bool editingActive = false;
    size_t carPos = 0;
public:
    TextEdit() = default;
    void update(const int frameDelay);
    //
    void moveCarriageToCursor(const UIContext& context, const std::u32string& text,
        const Point nodePosition, const float glyphWidth);
    void edit(const UIContext& context, std::u32string& text, const Point nodeSize,
        const Point nodePosition, const float glyphWidth, const std::unique_ptr<mingui::Validator>& validator);
    //
    size_t getCarrigePosition() const noexcept { return carPos; }
    bool isEditingActive() const noexcept { return editingActive; }
private:
    MINGUI_DISABLE_COPY_AND_MOVE(TextEdit)
};

END_NAMESPACE_MINGUI
