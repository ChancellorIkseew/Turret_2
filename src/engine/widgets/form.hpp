#pragma once
#include <string>
#include "clickable.hpp"
#include "engine/widgets/form_editor/form_validator.hpp"
#include "engine/window/input/utf8/utf8.hpp"

class Form : public Clickable {
    static constexpr PixelCoord FORM_SIZE{100.0f, 20.0f};
    std::u32string text;
    std::unique_ptr<Validator> validator;
public:
    template<typename T>
    Form(T value, Validator* validator) :
        Clickable(PixelCoord(100, 20)), validator(validator), text(to_u32string(value)) { }

    Form(std::u32string text) : Clickable(FORM_SIZE), text(text) { }
    Form()                    : Clickable(FORM_SIZE) { }
    ~Form() final;

    void draw() final;
    void callback() final;
    const std::u32string& getText() const { return text; }
    std::u32string& getText() { return text; }
};
