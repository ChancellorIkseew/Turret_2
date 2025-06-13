#pragma once
#include <SDL3/SDL_events.h>
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
    
    //static void rebind(const BindName keyName, const SDL_Event& event);
private:
    friend MainWindow;
    static void update(const SDL_Event& event);
    static void reset();
};
