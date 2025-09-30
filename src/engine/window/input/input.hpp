#pragma once
#include <optional>
#include "binding.hpp"
#include "engine/coords/pixel_coord.hpp"

struct SDL_Window;
union SDL_Event;
class MainWindow;

class Input {
public:
    ///@brief Check any press/click.
    static bool active(const cString bindName);
    ///@brief Check only short press/click.
    static bool jactive(const cString bindName);

    ///@brief Coordinate in window.
    static PixelCoord getMouseCoord();
    ///@brief (none/up/down)
    static MouseWheelScroll getMouseWheelScroll();

    ///@brief Int code and input type(keyboard/mouse) of the last key/button press.
    /// Is used for controls rebinding, but can have other usages.
    static std::optional<Binding> getLastKeyPressed();
    static void resetLastKeyPressed();
    ///@brief Last symbol entered in any text field.
    static std::optional<char32_t> getLastSymbolEntered();
    ///@brief Start/stop checking.
    static void enableTextEnter(const bool flag);
private:
    friend MainWindow;
    static void init(SDL_Window* mainWindow);
    static void update(const SDL_Event& event);
    static void reset();
};
