#pragma once
#include "ui_config.hpp"
#include "engine/coords/pixel_coord.hpp" //temporary

class Audio;
class Cursor;
class Input;

START_NAMESPACE_MINGUI

class Clickable;
class Node;

//struct Point { float x = 0.0f, y = 0.0f; };
using Point = PixelCoord;

class UIContext {
    Point mousePosition;
    bool mouseClicked;
public:
    Audio& audio;
    Cursor& cursor;
    Input& input;
    UIContext(Audio& audio, Cursor& cursor, Input& input);
    //
    bool containsMouse(const Node& node) const noexcept;
    bool idled(const Clickable& node)    const noexcept;
    bool hovered(const Clickable& node)  const noexcept;
    bool clicked(const Node& node)       const noexcept;
    bool clickedOut(const Node& node)    const noexcept;
};

END_NAMESPACE_MINGUI
