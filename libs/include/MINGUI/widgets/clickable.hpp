#pragma once
#include <MINGUI/widgets/ui_node.hpp>

START_NAMESPACE_MINGUI

class Clickable : public Node {
protected:
    ButtonState state = ButtonState::idle;
public:
    Clickable(const Point size, const ButtonState state) : Node(size) { setState(state); }
    Clickable(const Point size)                          : Node(size) { }
    ~Clickable() override = default;
    //
    void draw(RenderQueue& queue) override;
    void callback(UIContext& context) override;
    void setState(const ButtonState state) { this->state = state; };
    const ButtonState getState() const { return state; }
};

END_NAMESPACE_MINGUI
