#include "MINGUI/core/ui_context.hpp"
//
#include "MINGUI/widgets/clickable.hpp"
#include "engine/window/input/input.hpp"

MINGUI

UIContext::UIContext(UIContextBridge& contextBridge) :
    mouseClicked(contextBridge.mouseClicked),
    mousePosition(contextBridge.mousePosition),
    contextBridge(contextBridge) { }

bool UIContext::containsMouse(const Node& node) const noexcept {
    return node.containsMouse(mousePosition);
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
