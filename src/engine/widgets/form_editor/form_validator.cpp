#include "form_validator.hpp"
//
#include <algorithm>
#include "engine/io/parser/validator.hpp"
#include "engine/io/utf8/utf8.hpp"

static inline bool isNotUnsignedIntegral(const char32_t symbol) {
    return symbol < U'0' || symbol > U'9';
}
static inline bool isNotIntegral(const char32_t symbol) {
    return isNotUnsignedIntegral(symbol) && symbol != U'-';
}
static inline bool isNotFloat(const char32_t symbol) {
    return isNotIntegral(symbol) && symbol != U'.';
}
static inline bool isNotANSI(const char32_t symbol) {
    return symbol < 0 || symbol > 255;
}

static void validateUnsignedIntegral(std::u32string& text) {
    const auto it = std::remove_if(text.begin(), text.end(), isNotUnsignedIntegral);
    text.erase(it, text.end());
}
static void validateIntegral(std::u32string& text) {
    const auto it = std::remove_if(text.begin(), text.end(), isNotIntegral);
    text.erase(it, text.end());
}

bool Uint64Validator::isValid(const char32_t symbol) const { return !isNotUnsignedIntegral(symbol); }
bool Uint32Validator::isValid(const char32_t symbol) const { return !isNotUnsignedIntegral(symbol); }
bool Uint16Validator::isValid(const char32_t symbol) const { return !isNotUnsignedIntegral(symbol); }
bool Uint8Validator::isValid(const char32_t symbol) const { return !isNotUnsignedIntegral(symbol); }

bool Int64Validator::isValid(const char32_t symbol) const { return !isNotIntegral(symbol); }
bool Int32Validator::isValid(const char32_t symbol) const { return !isNotIntegral(symbol); }
bool Int16Validator::isValid(const char32_t symbol) const { return !isNotIntegral(symbol); }
bool Int8Validator::isValid(const char32_t symbol) const { return !isNotIntegral(symbol); }

void Uint64Validator::validateText(std::u32string& text) const { validateUnsignedIntegral(text); }
void Uint32Validator::validateText(std::u32string& text) const { validateUnsignedIntegral(text); }
void Uint16Validator::validateText(std::u32string& text) const { validateUnsignedIntegral(text); }
void Uint8Validator::validateText(std::u32string& text)  const { validateUnsignedIntegral(text); }

void Int64Validator::validateText(std::u32string& text) const { validateIntegral(text); }
void Int32Validator::validateText(std::u32string& text) const { validateIntegral(text); }
void Int16Validator::validateText(std::u32string& text) const { validateIntegral(text); }
void Int8Validator::validateText(std::u32string& text)  const { validateIntegral(text); }

void Uint64Validator::validateValue(std::u32string& text) const {
    const uint64_t value = validator::toUint64(text).value_or(0);
    text = utf8::to_u32string(std::clamp(value, min, max));
}
void Uint32Validator::validateValue(std::u32string& text) const {
    const uint32_t value = validator::toUint32(text).value_or(0);
    text = utf8::to_u32string(std::clamp(value, min, max));
}
void Uint16Validator::validateValue(std::u32string& text) const {
    const uint16_t value = validator::toUint16(text).value_or(0);
    text = utf8::to_u32string(std::clamp(value, min, max));
}
void Uint8Validator::validateValue(std::u32string& text) const {
    const uint8_t value = validator::toUint8(text).value_or(0);
    text = utf8::to_u32string(std::clamp(value, min, max));
}

void Int64Validator::validateValue(std::u32string& text) const {
    const int64_t value = validator::toInt64(text).value_or(0);
    text = utf8::to_u32string(std::clamp(value, min, max));
}
void Int32Validator::validateValue(std::u32string& text) const {
    const int32_t value = validator::toInt32(text).value_or(0);
    text = utf8::to_u32string(std::clamp(value, min, max));
}
void Int16Validator::validateValue(std::u32string& text) const {
    const int16_t value = validator::toInt16(text).value_or(0);
    text = utf8::to_u32string(std::clamp(value, min, max));
}
void Int8Validator::validateValue(std::u32string& text) const {
    const int8_t value = validator::toInt8(text).value_or(0);
    text = utf8::to_u32string(std::clamp(value, min, max));
}

bool FloatValidator::isValid(const char32_t symbol) const { return !isNotFloat(symbol); }
void FloatValidator::validateText(std::u32string& text) const {
    const auto it = std::remove_if(text.begin(), text.end(), isNotFloat);
    text.erase(it, text.end());
}
void FloatValidator::validateValue(std::u32string& text) const {
    const float value = validator::toFloat(text).value_or(0.0f);
    text = utf8::to_u32string(std::clamp(value, min, max));
}

bool ANSIValidator::isValid(const char32_t symbol) const { return !isNotANSI(symbol); }
void ANSIValidator::validateText(std::u32string& text) const {
    const auto it = std::remove_if(text.begin(), text.end(), isNotANSI);
    text.erase(it, text.end());
}
void ANSIValidator::validateValue(std::u32string& text) const {
    ANSIValidator::validateText(text);
}
