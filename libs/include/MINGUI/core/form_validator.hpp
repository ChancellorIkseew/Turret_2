#pragma once
#include <cstdint>
#include <string>
#include <MINGUI/core/config.hpp>

START_NAMESPACE_MINGUI

struct Validator {
    Validator() = default;
    virtual ~Validator() = default;
    virtual bool isValid(const char32_t symbol) const = 0;
    virtual void validateText(std::u32string& text) const = 0;
    virtual void validateValue(std::u32string& text) const = 0;
};

END_NAMESPACE_MINGUI
