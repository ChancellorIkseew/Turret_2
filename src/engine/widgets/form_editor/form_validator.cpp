#include "form_validator.hpp"
//
#include <algorithm>
#include "engine/io/parser/validator.hpp"

void Uint64Validator::validate(std::u32string& text) const {
    const uint64_t value = validator::toUint64(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Uint32Validator::validate(std::u32string& text) const {
    const uint32_t value = validator::toUint32(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Uint16Validator::validate(std::u32string& text) const {
    const uint16_t value = validator::toUint16(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Uint8Validator::validate(std::u32string& text) const {
    const uint8_t value = validator::toUint8(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}

void Int64Validator::validate(std::u32string& text) const {
    const int64_t value = validator::toInt64(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Int32Validator::validate(std::u32string& text) const {
    const int32_t value = validator::toInt32(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Int16Validator::validate(std::u32string& text) const {
    const int16_t value = validator::toInt16(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
void Int8Validator::validate(std::u32string& text) const {
    const int8_t value = validator::toInt8(text).value_or(0);
    text = to_u32string(std::clamp(value, min, max));
}
