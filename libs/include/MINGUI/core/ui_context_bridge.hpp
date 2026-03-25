#pragma once
#include <optional>
#include <MINGUI/core/config.hpp>
#include <MINGUI/core/point.hpp>

START_NAMESPACE_MINGUI

class Clickable;

struct TextInput {
    std::optional<char32_t> lastSymbolEntered;
    bool delete_     = false;
    bool backspace_  = false;
    bool arrowLeft_  = false;
    bool arrowRight_ = false;
    //TODO: arrowUp, arrowDown, cut, copy, paste
};

class UIContextBridge {
public:
    TextInput textInput;
    Point mousePosition;
    bool mouseClicked;
    //
    virtual ~UIContextBridge() = default;
    //
    virtual void onIdled(Clickable& node) = 0;
    virtual void onHovered(Clickable& node) = 0;
    virtual void onClicked(Clickable& node) = 0;
    virtual void onChecked(Clickable& node) = 0;
};

END_NAMESPACE_MINGUI
