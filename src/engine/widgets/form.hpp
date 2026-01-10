#pragma once
#include <memory>
#include <string>
#include "clickable.hpp"
#include "engine/io/utf8/utf8.hpp"
#include "engine/widgets/form_validator/form_validator.hpp"

class Form : public Clickable {
    static constexpr PixelCoord DEFAULT_FORM_SIZE{100.0f, 20.0f};
    std::u32string text;
    std::unique_ptr<Validator> validator;
    PixelCoord glyphSize = PixelCoord(16.0f, 16.0f);
    size_t carrigePosition = 0;
public:
    template<typename T>
    Form(T value, Validator* validator, const PixelCoord size = DEFAULT_FORM_SIZE) :
        Clickable(size), validator(validator), text(utf8::to_u32string(value)) { }

    Form(std::u32string text, const PixelCoord size = DEFAULT_FORM_SIZE) : Clickable(size), text(text) { }
    Form(const PixelCoord size = DEFAULT_FORM_SIZE)                      : Clickable(size) { }
    ~Form() final = default;

    void draw(const Renderer& renderer) final;
    void callback(UIContext& context) final;
    void setGlyphSize(const PixelCoord size) { glyphSize = size; }
    const std::u32string& getText() const { return text; }
    std::u32string& getText() { return text; }
};
