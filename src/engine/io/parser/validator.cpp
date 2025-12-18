#include "validator.hpp"
//
#include <stdexcept>
#include "engine/coords/pixel_coord.hpp"
#include "engine/coords/tile_coord.hpp"

// Unsigned int
std::optional<uint64_t> validator::toUint64(const std::string& str) {
    try {
        return std::stoull(str);
    }
    catch (const std::logic_error&) { // out_of_range | invalid_argument
        return std::nullopt;
    }
}

template <typename T>
std::optional<T> toUint(const std::string& str) {
    const auto v = validator::toUint64(str);
    if (v.has_value() && v.value() <= std::numeric_limits<T>::max())
        return static_cast<T>(v.value());
    return std::nullopt;
}

std::optional<uint32_t> validator::toUint32(const std::string& str) {
    return toUint<uint32_t>(str);
}
std::optional<uint16_t> validator::toUint16(const std::string& str) {
    return toUint<uint16_t>(str);
}
std::optional<uint8_t> validator::toUint8(const std::string& str) {
    return toUint<uint8_t>(str);
}

// Signed int
std::optional<int64_t> validator::toInt64(const std::string& str) {
    try {
        return std::stoll(str);
    }
    catch (const std::logic_error&) { // out_of_range | invalid_argument
        return std::nullopt;
    }
}

template <typename T>
std::optional<T> toInt(const std::string& str) {
    const auto v = validator::toInt64(str);
    if (v.has_value() && v.value() >= std::numeric_limits<T>::min() && v.value() <= std::numeric_limits<T>::max())
        return static_cast<T>(v.value());
    return std::nullopt;
}

std::optional<int32_t> validator::toInt32(const std::string& str) {
    return toInt<int32_t>(str);
}
std::optional<int16_t> validator::toInt16(const std::string& str) {
    return toInt<int16_t>(str);
}
std::optional<int8_t> validator::toInt8(const std::string& str) {
    return toInt<int8_t>(str);
}

//other
std::optional<TileCoord> validator::toTileCoord(const std::string& str) {
    const size_t stick = str.find_first_of('|');
    if (stick == std::string::npos)
        return std::nullopt;
    const auto x = validator::toInt32(str.substr(0, stick));
    const auto y = validator::toInt32(str.substr(stick + 1));
    if (!x.has_value() || !y.has_value())
        return std::nullopt;
    return TileCoord(x.value(), y.value());
}

std::optional<PixelCoord> validator::toPixelCoord(const std::string& str) {
    const size_t stick = str.find_first_of('|');
    if (stick == std::string::npos)
        return std::nullopt;
    const auto x = validator::toFloat(str.substr(0, stick));
    const auto y = validator::toFloat(str.substr(stick + 1));
    if (!x.has_value() || !y.has_value())
        return std::nullopt;
    return PixelCoord(x.value(), y.value());
}

std::optional<float> validator::toFloat(const std::string& str) {
    try {
        return std::stof(str);
    }
    catch (const std::logic_error&) { // out_of_range | invalid_argument
        return std::nullopt;
    }
}

std::optional<bool> validator::toBool(const std::string& str) {
    if (str == "true" || str == "1")
        return true;
    if (str == "false" || str == "0")
        return false;
    return std::nullopt;
}

std::string validator::trimToStdString(const std::u32string& u32str) {
    if (u32str.empty())
        return "";
    std::string str;
    str.reserve(u32str.size());
    for (const auto it : u32str) {
        str.push_back(static_cast<char>(it));
    }
    return str;
}
