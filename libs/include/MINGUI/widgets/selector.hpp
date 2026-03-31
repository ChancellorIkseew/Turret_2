#pragma once
#include <MINGUI/widgets/abstract_button.hpp>
#include <MINGUI/widgets/layout.hpp>

START_NAMESPACE_MINGUI

class Selector : public Layout {
    std::weak_ptr<AbstractButton> target;
public:
    Selector(const Orientation orientation) : Layout(orientation) { }
    ~Selector() override = default;
    //
    const std::weak_ptr<AbstractButton> getTarget() const { return target; }
    void setTarget(const AbstractButton* const button);
    void resetTarget();
    bool isTarget(const AbstractButton* const button) const;
    void callback(UIContext& context) final;
private:
    void setTarget(std::shared_ptr<AbstractButton> button);
};

END_NAMESPACE_MINGUI
