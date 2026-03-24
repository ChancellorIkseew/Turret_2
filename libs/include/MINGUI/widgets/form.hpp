#pragma once
#include <memory>
#include <string>
#include "clickable.hpp"
#include "MINGUI/core/utf8.hpp"
#include "MINGUI/core/form_validator.hpp"

START_NAMESPACE_MINGUI

class Form : public Clickable {
    static constexpr Point DEFAULT_FORM_SIZE{100.0f, 20.0f};
    std::u32string text;
    std::unique_ptr<Validator> validator;
    Point glyphSize = Point(16.0f, 16.0f);
    size_t carrigePosition = 0;
public:
    template<typename T>
    Form(T value, Validator* validator, const Point size = DEFAULT_FORM_SIZE) :
        Clickable(size), validator(validator), text(utf8::to_u32string(value)) { }

    Form(std::u32string text, const Point size = DEFAULT_FORM_SIZE) : Clickable(size), text(text) { }
    Form(const Point size = DEFAULT_FORM_SIZE)                      : Clickable(size) { }
    ~Form() final = default;

    void draw(RenderQueue& queue) final;
    void callback(UIContext& context) final;
    void setGlyphSize(const Point size) { glyphSize = size; }
    const std::u32string& getText() const { return text; }
    std::u32string& getText() { return text; }
};

END_NAMESPACE_MINGUI
