#pragma once
#include <MINGUI/widgets/abstract_button.hpp>
#include <MINGUI/widgets/label.hpp>

START_NAMESPACE_MINGUI

class Button : public AbstractButton {
    Label label;
public:
    Button(const Point size, const std::string& name, const bool translateble = true)
        : AbstractButton(size), label(name, translateble) { }
    ~Button() final = default;
    //
    void draw(RenderQueue& queue) final;
    void translate(const Localization& localization) final;
    void setPosition(const Point position) final;
    void setText(const std::string& name);
private:
    void centerText();
};

END_NAMESPACE_MINGUI
