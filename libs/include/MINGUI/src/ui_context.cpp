#include <MINGUI/core/ui_context.hpp>
//
#include <MINGUI/widgets/clickable.hpp>

MINGUI

UIContext::UIContext(UIContextBridge& contextBridge, TextEdit& textEdit, const float canvasScale) :
    mouseClicked(contextBridge.mouseClicked),
    mousePosition(contextBridge.mousePosition),
    canvasScale(canvasScale),
    contextBridge(contextBridge),
    textEdit(textEdit) { }

bool UIContext::containsMouse(const Node& node) const noexcept {
    return node.containsMouse(mousePosition / canvasScale);
}

bool UIContext::idled(const Clickable& node) const noexcept {
    return !containsMouse(node) && node.getState() == ButtonState::hover;
}

bool UIContext::hovered(const Clickable& node) const noexcept {
    return containsMouse(node) && node.getState() == ButtonState::idle;
}

bool UIContext::clicked(const Node& node) const noexcept {
    return mouseClicked && containsMouse(node);
}

bool UIContext::clickedOut(const Node& node) const noexcept {
    return mouseClicked && !containsMouse(node);
}
