#include "form_validator.hpp"
//
#include <algorithm>
#include "engine/io/parser/validator.hpp"

static inline bool isNotUnsignedIntegral(const uint32_t symbol) {
    return symbol < '0' || symbol > '9';
}
static inline bool isNotIntegral(const uint32_t symbol) {
    return isNotUnsignedIntegral(symbol) && symbol != '-';
}
static inline bool isNotFloat(const uint8_t symbol) {
    return isNotIntegral(symbol) && symbol != '.';
}

static void validateUnsignedIntegral(std::u32string& text) {
    const auto it = std::remove_if(text.begin(), text.end(), isNotUnsignedIntegral);
    text.erase(it);
}
static void validateIntegral(std::u32string& text) {
    const auto it = std::remove_if(text.begin(), text.end(), isNotIntegral);
    text.erase(it);
}

void Uint64Validator::validateText(std::u32string& text) const { validateUnsignedIntegral(text); }
void Uint32Validator::validateText(std::u32string& text) const { validateUnsignedIntegral(text); }
void Uint16Validator::validateText(std::u32string& text) const { validateUnsignedIntegral(text); }
void Uint8Validator::validateText(std::u32string& text) const  { validateUnsignedIntegral(text); }

void Int64Validator::validateText(std::u32string& text) const { validateIntegral(text); }
void Int32Validator::validateText(std::u32string& text) const { validateIntegral(text); }
void Int16Validator::validateText(std::u32string& text) const { validateIntegral(text); }
void Int8Validator::validateText(std::u32string& text)  const { validateIntegral(text); }

void Uint64Validator::validateValue(std::u32string& text) const {
    const uint64_t value = validator::toUint64(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Uint32Validator::validateValue(std::u32string& text) const {
    const uint32_t value = validator::toUint32(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Uint16Validator::validateValue(std::u32string& text) const {
    const uint16_t value = validator::toUint16(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Uint8Validator::validateValue(std::u32string& text) const {
    const uint8_t value = validator::toUint8(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}

void Int64Validator::validateValue(std::u32string& text) const {
    const int64_t value = validator::toInt64(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Int32Validator::validateValue(std::u32string& text) const {
    const int32_t value = validator::toInt32(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Int16Validator::validateValue(std::u32string& text) const {
    const int16_t value = validator::toInt16(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Int8Validator::validateValue(std::u32string& text) const {
    const int8_t value = validator::toInt8(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
