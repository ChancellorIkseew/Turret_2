#pragma once
#include <SDL3/SDL_events.h>
#include <optional>
#include <string>
#include "binding.hpp"
#include "engine/coords/pixel_coord.hpp"

class MainWindow;

class Input {
public:
    ///@brief Check any press/click.
    static bool active(const BindName bindName);
    ///@brief Check only short press/click.
    static bool jactive(const BindName bindName);

    ///@brief Coordinate in window.
    static PixelCoord getMouseCoord();
    ///@brief (none/up/down)
    static MouseWheelScroll getMouseWheelScroll();

    ///@brief Int code and input type(keyboard/mouse) of the last key/button press.
    /// Is used for controls rebinding, but can have other usages.
    static std::optional<BindingInfo> getLastKeyPressed();
    static void resetLastKeyPressed();
    ///@brief Last symbol entered in any text field.
    static std::optional<uint32_t> getLastSymbolEntered();
    ///@brief Start/stop checking.
    static void enanleTextEnter(const bool flag);
private:
    friend MainWindow;
    static void init(SDL_Window* mainWindow);
    static void update(const SDL_Event& event);
    static void reset();
};
