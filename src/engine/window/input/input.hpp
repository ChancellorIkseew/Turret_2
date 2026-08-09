#pragma once
#include <optional>
#include <string_view>
#include "binding.hpp"
#include "engine/coords/pixel_coord.hpp"

struct SDL_Window;
union SDL_Event;
class MainWindow;

class Input {
    std::optional<Binding> lastKeyPressed;
    std::string_view textEntered;
    PixelCoord mouseCoord;
    MouseWheelScroll mouseWheelScroll = MouseWheelScroll::none;
    bool textInputActive = false;
public:
    Input() = default;

    ///@brief Check any press/click.
    bool active(cString bindName) const;
    ///@brief Check only short press/click.
    bool jactive(cString bindName) const;
    ///@brief Check any press/click ended.
    bool released(cString bindName) const;

    ///@brief Coordinate in window.
    PixelCoord getMouseCoord() const { return mouseCoord; }
    ///@brief (none/up/down)
    MouseWheelScroll getMouseWheelScroll() const { return mouseWheelScroll; }

    ///@brief Int code and input type(keyboard/mouse) of the last key/button press.
    /// Is used for controls rebinding, but can have other usages.
    std::optional<Binding> getLastKeyPressed() const { return lastKeyPressed; }
    std::string_view getTextEntered() const { return textEntered; }
    ///@brief start/stop checking.
    void enableTextInput(const bool flag) { textInputActive = flag; }

    void consumeMouseClicks();
private:
    friend MainWindow;
    void update(const SDL_Event& event);
    void reset(SDL_Window* sdlWindow);
    t1_disable_copy_and_move(Input)
};
