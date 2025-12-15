#pragma once
#include "button.hpp"
#include "layout.hpp"

class Selector : public Layout {
    std::weak_ptr<Button> target;
public:
    Selector(const Orientation orientation) : Layout(orientation) { }
    ~Selector() final = default;
    //
    const std::weak_ptr<Button> getTarget() const { return target; }
    void setTarget(const Button* const button);
    void resetTarget() { target.reset(); }
    void callback() final;
private:
    void setTarget(std::shared_ptr<Button> button);
};
