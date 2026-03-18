#pragma once
#include <cstdint>
#include <string>

struct Validator {
    Validator() = default;
    virtual ~Validator() = default;
    virtual bool isValid(const char32_t symbol) const = 0;
    virtual void validateText(std::u32string& text) const = 0;
    virtual void validateValue(std::u32string& text) const = 0;
};
