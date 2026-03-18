#pragma once
#include "ui_config.hpp"
#include "engine/coords/pixel_coord.hpp" //temporary

START_NAMESPACE_MINGUI

//struct Point { float x = 0.0f, y = 0.0f; };
using Point = PixelCoord;

class Clickable;

class UIContextBridge {
public:
    Point mousePosition;
    bool mouseClicked;

    virtual void onIdled(Clickable& node) = 0;
    virtual void onHovered(Clickable& node) = 0;
    virtual void onClicked(Clickable& node) = 0;
    virtual void onChecked(Clickable& node) = 0;
};

END_NAMESPACE_MINGUI
