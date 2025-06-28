#pragma once
#include <SDL3/SDL_events.h>
#include <optional>
#include <unordered_map>
#include "binding.hpp"
#include "engine/coords/pixel_coord.hpp"

class MainWindow;

class Input {
    static std::unordered_map<BindName, Binding> bindings;
public:
    ///@brief Check any press/click.
    static bool active(const BindName bindName);
    ///@brief Check only short press/click.
    static bool jactive(const BindName bindName);

    ///@brief Coordinate in window.
    static PixelCoord getMouseCoord();
    ///@brief (none/up/down)
    static MouseWheelScroll getMouseWheelScroll();

    ///@brief Last symbol entered in any text field.
    static std::optional<uint32_t> getLastSymbolEntered();
    ///@brief Start/stop checking.
    static void enanleTextEnter(const bool flag);

    //static void rebind(const BindName keyName, const SDL_Event& event);
private:
    friend MainWindow;
    static void init(SDL_Window* mainWindow);
    static void update(const SDL_Event& event);
    static void reset();
};
