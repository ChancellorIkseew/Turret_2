#include "form_validator.hpp"
//
#include "MINGUI/core/utf8.hpp"
#include <algorithm>
#include <format>
#include "engine/io/parser/validator.hpp"

static inline constexpr bool isNotUnsignedIntegral(const char32_t symbol) {
    return symbol < U'0' || symbol > U'9';
}
static inline constexpr bool isNotIntegral(const char32_t symbol) {
    return isNotUnsignedIntegral(symbol) && symbol != U'-';
}
static inline constexpr bool isNotFloat(const char32_t symbol) {
    return isNotIntegral(symbol) && symbol != U'.';
}
static inline constexpr bool isNotANSI(const char32_t symbol) {
    return symbol < 0 || symbol > 255;
}

static void validateUnsignedIntegral(std::string& text) {
    std::erase_if(text, isNotUnsignedIntegral);
}
static void validateIntegral(std::string& text) {
    std::erase_if(text, isNotIntegral);
}

bool Uint64Validator::isValid(const char32_t symbol) const { return !isNotUnsignedIntegral(symbol); }
bool Uint32Validator::isValid(const char32_t symbol) const { return !isNotUnsignedIntegral(symbol); }
bool Uint16Validator::isValid(const char32_t symbol) const { return !isNotUnsignedIntegral(symbol); }
bool Uint8Validator::isValid(const char32_t symbol) const { return !isNotUnsignedIntegral(symbol); }

bool Int64Validator::isValid(const char32_t symbol) const { return !isNotIntegral(symbol); }
bool Int32Validator::isValid(const char32_t symbol) const { return !isNotIntegral(symbol); }
bool Int16Validator::isValid(const char32_t symbol) const { return !isNotIntegral(symbol); }
bool Int8Validator::isValid(const char32_t symbol) const { return !isNotIntegral(symbol); }

void Uint64Validator::validateText(std::string& text) const { validateUnsignedIntegral(text); }
void Uint32Validator::validateText(std::string& text) const { validateUnsignedIntegral(text); }
void Uint16Validator::validateText(std::string& text) const { validateUnsignedIntegral(text); }
void Uint8Validator::validateText(std::string& text)  const { validateUnsignedIntegral(text); }

void Int64Validator::validateText(std::string& text) const { validateIntegral(text); }
void Int32Validator::validateText(std::string& text) const { validateIntegral(text); }
void Int16Validator::validateText(std::string& text) const { validateIntegral(text); }
void Int8Validator::validateText(std::string& text)  const { validateIntegral(text); }

void Uint64Validator::validateValue(std::string& text) const {
    const uint64_t value = validator::to<uint64_t>(text).value_or(0);
    text = std::format("{}", std::clamp(value, min, max));
}
void Uint32Validator::validateValue(std::string& text) const {
    const uint32_t value = validator::to<uint32_t>(text).value_or(0);
    text = std::format("{}", std::clamp(value, min, max));
}
void Uint16Validator::validateValue(std::string& text) const {
    const uint16_t value = validator::to<uint16_t>(text).value_or(0);
    text = std::format("{}", std::clamp(value, min, max));
}
void Uint8Validator::validateValue(std::string& text) const {
    const uint8_t value = validator::to<uint8_t>(text).value_or(0);
    text = std::format("{}", std::clamp(value, min, max));
}

void Int64Validator::validateValue(std::string& text) const {
    const int64_t value = validator::to<int64_t>(text).value_or(0);
    text = std::format("{}", std::clamp(value, min, max));
}
void Int32Validator::validateValue(std::string& text) const {
    const int32_t value = validator::to<int32_t>(text).value_or(0);
    text = std::format("{}", std::clamp(value, min, max));
}
void Int16Validator::validateValue(std::string& text) const {
    const int16_t value = validator::to<int16_t>(text).value_or(0);
    text = std::format("{}", std::clamp(value, min, max));
}
void Int8Validator::validateValue(std::string& text) const {
    const int8_t value = validator::to<int8_t>(text).value_or(0);
    text = std::format("{}", std::clamp(value, min, max));
}

bool FloatValidator::isValid(const char32_t symbol) const { return !isNotFloat(symbol); }
void FloatValidator::validateText(std::string& text) const {
    std::erase_if(text, isNotFloat);
}
void FloatValidator::validateValue(std::string& text) const {
    const float value = validator::to<float>(text).value_or(0.f);
    text = std::format("{}", std::clamp(value, min, max));
}

bool ANSIValidator::isValid(const char32_t symbol) const { return !isNotANSI(symbol); }
void ANSIValidator::validateText(std::string& text) const {
    std::erase_if(text, isNotANSI);
}
void ANSIValidator::validateValue(std::string& text) const {
    std::erase_if(text, isNotANSI);
}
