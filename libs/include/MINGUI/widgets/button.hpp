#pragma once
#include <MINGUI/widgets/abstract_button.hpp>
#include <MINGUI/widgets/label.hpp>

START_NAMESPACE_MINGUI

class Button : public AbstractButton {
    Label label;
public:
    Button(const Point size, const std::string& name)
        : AbstractButton(size), label(name) { centerText(); }
    ~Button() final = default;
    //
    void draw(RenderQueue& queue) final;
    void setPosition(const Point position) final;
    void setText(const std::string& name);
private:
    void centerText();
};

END_NAMESPACE_MINGUI
