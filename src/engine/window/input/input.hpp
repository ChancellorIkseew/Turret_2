#pragma once
#include <optional>
#include "binding.hpp"
#include "engine/coords/pixel_coord.hpp"

struct SDL_Window;
union SDL_Event;
class MainWindow;

class Input {
    SDL_Window* sdlWindow;
    std::optional<Binding> lastKeyPressed;
    std::optional<char32_t> symbolJustEntered;
    PixelCoord mouseCoord;
    MouseWheelScroll mouseWheelScroll = MouseWheelScroll::none;
public:
    Input(SDL_Window* sdlWindow) : sdlWindow(sdlWindow) { }

    ///@brief Check any press/click.
    bool active(const cString bindName) const;
    ///@brief Check only short press/click.
    bool jactive(const cString bindName) const;

    ///@brief Coordinate in window.
    PixelCoord getMouseCoord() const;
    ///@brief (none/up/down)
    MouseWheelScroll getMouseWheelScroll() const;

    ///@brief Int code and input type(keyboard/mouse) of the last key/button press.
    /// Is used for controls rebinding, but can have other usages.
    std::optional<Binding> getLastKeyPressed() const;
    ///@brief Last symbol entered in any text field.
    std::optional<char32_t> getLastSymbolEntered() const;
    ///@brief Start/stop checking.
    void enableTextEnter(const bool flag);
    bool isTextEnterEnabled() const;
private:
    friend MainWindow;
    void update(const SDL_Event& event);
    void reset();
private:
    Input(const Input&) = delete;
    Input(Input&&) = delete;
    Input& operator=(const Input&) = delete;
    Input& operator=(Input&&) = delete;
};
