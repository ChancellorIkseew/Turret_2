#pragma once
#include <cstdint>
#include <optional>
#include <string>

struct PixelCoord;
struct TileCoord;

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
    std::optional<TileCoord> toTileCoord(const std::string& str);
    std::optional<PixelCoord> toPixelCoord(const std::string& str);
    std::optional<float> toFloat(const std::string& str);
    std::optional<bool> toBool(const std::string& str);
    std::string trimToStdString(const std::u32string& str);

    inline std::optional<uint64_t> toUint64(const std::u32string& str) { return toUint64(trimToStdString(str)); }
    inline std::optional<uint32_t> toUint32(const std::u32string& str) { return toUint32(trimToStdString(str)); }
    inline std::optional<uint16_t> toUint16(const std::u32string& str) { return toUint16(trimToStdString(str)); }
    inline std::optional<uint8_t> toUint8(const std::u32string& str) { return toUint8(trimToStdString(str)); }
    //
    inline std::optional<int64_t> toInt64(const std::u32string& str) { return toInt64(trimToStdString(str)); }
    inline std::optional<int32_t> toInt32(const std::u32string& str) { return toInt32(trimToStdString(str)); }
    inline std::optional<int16_t> toInt16(const std::u32string& str) { return toInt16(trimToStdString(str)); }
    inline std::optional<int8_t> toInt8(const std::u32string& str) { return toInt8(trimToStdString(str)); }
    //
    inline std::optional<float> toFloat(const std::u32string& str) { return toFloat(trimToStdString(str)); }
}
