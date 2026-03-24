#pragma once
#include <functional>
#include "clickable.hpp"
#include "label.hpp"

START_NAMESPACE_MINGUI

class Button : public Clickable {
    std::function<void()> action;
    Label label;
public:
    Button(const Point size, const std::string& name, const bool translateble = true)
        : Clickable(size), label(name, translateble) { }
    ~Button() final = default;
    //
    void draw(RenderQueue& queue) final;
    void callback(UIContext& context) final;
    void setPosition(const Point position) final;
    void setText(const std::string& name);
    void translate(const Localization& localization) final {
        label.translate(localization);
        centerText();
    }
    void addCallback(std::function<void()> action) {
        this->action = action;
    }
private:
    void centerText();
};

END_NAMESPACE_MINGUI
