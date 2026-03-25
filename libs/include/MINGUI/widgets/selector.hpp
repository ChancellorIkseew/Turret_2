#pragma once
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/layout.hpp>

START_NAMESPACE_MINGUI

class Selector : public Layout {
    std::weak_ptr<Button> target;
public:
    Selector(const Orientation orientation) : Layout(orientation) { }
    ~Selector() override = default;
    //
    const std::weak_ptr<Button> getTarget() const { return target; }
    void setTarget(const Button* const button);
    void resetTarget() { target.reset(); }
    void callback(UIContext& context) final;
private:
    void setTarget(std::shared_ptr<Button> button);
};

END_NAMESPACE_MINGUI
