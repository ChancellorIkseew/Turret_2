#pragma once
#include "ui_config.hpp"
#include "point.hpp"

START_NAMESPACE_MINGUI

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
