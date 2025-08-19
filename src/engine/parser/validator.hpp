#pragma once
#include <optional>
#include <string>

namespace validator {
    std::optional<uint64_t> toUint64(const std::string& str);
    std::optional<uint32_t> toUint32(const std::string& str);
    std::optional<uint16_t> toUint16(const std::string& str);
    std::optional<uint8_t> toUint8(const std::string& str);
    //
    std::optional<int64_t> toInt64(const std::string& str);
    std::optional<int32_t> toInt32(const std::string& str);
    std::optional<int16_t> toInt16(const std::string& str);
    std::optional<int8_t> toInt8(const std::string& str);
    //
    std::optional<float> toFloat(const std::string& str);
    std::optional<bool> toBool(const std::string& str);
    std::string toStdString(const std::u32string& str);
}
