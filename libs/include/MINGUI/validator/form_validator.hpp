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

struct Uint64Validator : Validator {
    const uint64_t min, max;
    Uint64Validator(const uint64_t min, const uint64_t max) : min(min), max(max) { }
    ~Uint64Validator() final = default;
    bool isValid(const char32_t symbol) const final;
    void validateText(std::u32string& text) const final;
    void validateValue(std::u32string& text) const final;
};
struct Uint32Validator : Validator {
    const uint32_t min, max;
    Uint32Validator(const uint32_t min, const uint32_t max) : min(min), max(max) {}
    ~Uint32Validator() final = default;
    bool isValid(const char32_t symbol) const final;
    void validateText(std::u32string& text) const final;
    void validateValue(std::u32string& text) const final;
};
struct Uint16Validator : Validator {
    const uint16_t min, max;
    Uint16Validator(const uint16_t min, const uint16_t max) : min(min), max(max) {}
    ~Uint16Validator() final = default;
    bool isValid(const char32_t symbol) const final;
    void validateText(std::u32string& text) const final;
    void validateValue(std::u32string& text) const final;
};
struct Uint8Validator : Validator {
    const uint8_t min, max;
    Uint8Validator(const uint8_t min, const uint8_t max) : min(min), max(max) {}
    ~Uint8Validator() final = default;
    bool isValid(const char32_t symbol) const final;
    void validateText(std::u32string& text) const final;
    void validateValue(std::u32string& text) const final;
};

struct Int64Validator : Validator {
    const int64_t min, max;
    Int64Validator(const int64_t min, const int64_t max) : min(min), max(max) {}
    ~Int64Validator() final = default;
    bool isValid(const char32_t symbol) const final;
    void validateText(std::u32string& text) const final;
    void validateValue(std::u32string& text) const final;
};
struct Int32Validator : Validator {
    const int32_t min, max;
    Int32Validator(const int32_t min, const int32_t max) : min(min), max(max) {}
    ~Int32Validator() final = default;
    bool isValid(const char32_t symbol) const final;
    void validateText(std::u32string& text) const final;
    void validateValue(std::u32string& text) const final;
};
struct Int16Validator : Validator {
    const int16_t min, max;
    Int16Validator(const int16_t min, const int16_t max) : min(min), max(max) {}
    ~Int16Validator() final = default;
    bool isValid(const char32_t symbol) const final;
    void validateText(std::u32string& text) const final;
    void validateValue(std::u32string& text) const final;
};
struct Int8Validator : Validator {
    const int8_t min, max;
    Int8Validator(const int8_t min, const int8_t max) : min(min), max(max) {}
    ~Int8Validator() final = default;
    bool isValid(const char32_t symbol) const final;
    void validateText(std::u32string& text) const final;
    void validateValue(std::u32string& text) const final;
};

struct FloatValidator : Validator {
    const float min, max;
    FloatValidator(const float min, const float max) : min(min), max(max) {}
    ~FloatValidator() final = default;
    bool isValid(const char32_t symbol) const final;
    void validateText(std::u32string& text) const final;
    void validateValue(std::u32string& text) const final;
};
struct ANSIValidator : Validator {
    ANSIValidator() = default;
    ~ANSIValidator() final = default;
    bool isValid(const char32_t symbol) const final;
    void validateText(std::u32string& text) const final;
    void validateValue(std::u32string& text) const final;
};
