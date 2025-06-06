#pragma once
#include <unordered_map>
#include <SDL3/SDL_events.h>
#include "binding.hpp"
#include "coords/pixel_coord.hpp"

class Input {
    static std::unordered_map<BindName, Binding> bindings;
public:
    static void update(const SDL_Event& event);
    static void reset();

    ///@brief Check any press/click.
    static bool active(const BindName bindName);
    ///@brief Check only short press/click.
    static bool jactive(const BindName bindName);

    ///@brief Coordinate in window.
    static PixelCoord getMouseCoord();
    ///@brief (none/up/down)
    static MouseWheelScroll getMouseWheelScroll();
    
    //static void rebind(const BindName keyName, const SDL_Event& event);
};
