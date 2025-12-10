#pragma once
#include <memory>
#include <string>
#include "clickable.hpp"
#include "engine/io/utf8/utf8.hpp"
#include "engine/widgets/form_editor/form_validator.hpp"

class Form : public Clickable, public std::enable_shared_from_this<Form> {
    static constexpr PixelCoord FORM_SIZE{100.0f, 20.0f};
    std::u32string text;
    std::unique_ptr<Validator> validator;
public:
    template<typename T>
    Form(T value, Validator* validator) :
        Clickable(PixelCoord(100, 20)), validator(validator), text(utf8::to_u32string(value)) { }

    Form(std::u32string text) : Clickable(FORM_SIZE), text(text) { }
    Form()                    : Clickable(FORM_SIZE) { }
    ~Form() final = default;

    void draw() final;
    void callback() final;
    void validate();
    const std::u32string& getText() const { return text; }
    std::u32string& getText() { return text; }
    bool accepts(const char32_t symbol) {
        if (validator)
            return validator->isValid(symbol);
        return true;
    }
};
