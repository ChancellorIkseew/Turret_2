#pragma once
#include <optional>
#include "binding.hpp"
#include "engine/coords/pixel_coord.hpp"

struct SDL_Window;
union SDL_Event;
class MainWindow;

class Input {
    std::optional<Binding> lastKeyPressed;
    const char* symbolJustEntered = nullptr;
    PixelCoord mouseCoord;
    MouseWheelScroll mouseWheelScroll = MouseWheelScroll::none;
    bool textInputActive = false;
public:
    Input() = default;

    ///@brief Check any press/click.
    bool active(const cString bindName) const;
    ///@brief Check only short press/click.
    bool jactive(const cString bindName) const;
    ///@brief Check any press/click ended.
    bool released(const cString bindName) const;

    ///@brief Coordinate in window.
    PixelCoord getMouseCoord() const;
    ///@brief (none/up/down)
    MouseWheelScroll getMouseWheelScroll() const;

    ///@brief Int code and input type(keyboard/mouse) of the last key/button press.
    /// Is used for controls rebinding, but can have other usages.
    std::optional<Binding> getLastKeyPressed() const;
    ///@brief Last symbol entered in any text field.
    const char* getLastSymbolEntered() const;
    ///@brief start/stop checking.
    void enableTextInput(const bool flag) { textInputActive = flag; }
private:
    friend MainWindow;
    void update(const SDL_Event& event);
    void reset(SDL_Window* sdlWindow);
    t1_disable_copy_and_move(Input)
};
