#pragma once
#include <memory>
#include <string>
#include "config.hpp"
#include "engine/coords/pixel_coord.hpp"

struct SDL_Window;
struct Validator;
class Input;
class MainWindow;

class TextEdit {
    int inputTimer = 0;
    bool editingActive = false;
    size_t carPos = 0;
public:
    TextEdit() = default;
    //
    void moveCarriageToCursor(const Input& input, const std::u32string& text,
        const PixelCoord nodePosition, const float glyphWidth);
    void edit(const Input& input, std::u32string& text, const PixelCoord nodeSize,
        const PixelCoord nodePosition, const float glyphWidth, const std::unique_ptr<Validator>& validator);
    //
    size_t getCarrigePosition() const { return carPos; }
private:
    friend MainWindow;
    void update(SDL_Window* sdlWindow, const int frameDelay);
    t1_disable_copy_and_move(TextEdit)
};
