#pragma once
#include <MINGUI/core/ui_context_bridge.hpp>

START_NAMESPACE_MINGUI

class Clickable;
class Node;
class TextEdit;

class UIContext {
    Point mousePosition;
    bool mouseClicked;
    UIContextBridge& contextBridge;
    TextEdit& textEdit;
public:
    UIContext(UIContextBridge& contextBridge, TextEdit& textEdit);
    //
    bool  getMouseClicked()         const noexcept { return mouseClicked; }
    Point getMousePosition()        const noexcept { return mousePosition; }
    const TextInput& getTextInput() const noexcept { return contextBridge.textInput; }
    TextEdit& getTextEdit() noexcept { return textEdit; }
    //
    bool containsMouse(const Node& node) const noexcept;
    bool idled(const Clickable& node)    const noexcept;
    bool hovered(const Clickable& node)  const noexcept;
    bool clicked(const Node& node)       const noexcept;
    bool clickedOut(const Node& node)    const noexcept;
    //
    void onIdled(Clickable& node)   { contextBridge.onIdled(node); }
    void onHovered(Clickable& node) { contextBridge.onHovered(node); }
    void onClicked(Clickable& node) { contextBridge.onClicked(node); }
    void onChecked(Clickable& node) { contextBridge.onChecked(node); }
};

END_NAMESPACE_MINGUI
