#pragma once
#include <memory>
#include <string>
#include "engine/coords/pixel_coord.hpp"

struct Validator;
class Input;

class TextEdit {
    int inputTimer = 0;
    size_t carPos = 0;
public:
    TextEdit() = default;
    //
    void updateTimer(const int frameDelay);
    void moveCarriageToCursor(const Input& input, const std::u32string& text,
        const PixelCoord nodePosition, const float glyphWidth);
    void edit(const Input& input, std::u32string& text, const PixelCoord nodeSize,
        const PixelCoord nodePosition, const float glyphWidth, const std::unique_ptr<Validator>& validator);
    //
    size_t getCarrigePosition() const { return carPos; }
};
