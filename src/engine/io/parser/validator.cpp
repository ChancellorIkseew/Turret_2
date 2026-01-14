#include "validator.hpp"
//
#include <algorithm>
#include <charconv>
#include <limits>
#include <string_view>
#include <utility>

template <class T>
static std::optional<T> stringToNumber(std::string_view str) {
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);
    str.remove_prefix(std::min(str.find_first_not_of(" "), str.size()));
    if (str.empty())
        return std::nullopt;
    T value{};
    auto [ptr, errorCode] = std::from_chars(str.data(), str.data() + str.size(), value);
    if (errorCode == std::errc::invalid_argument)
        return std::nullopt;
    if (errorCode == std::errc::result_out_of_range) {
        if constexpr (!std::is_floating_point_v<T>) //float
            return std::numeric_limits<T>::max();
        if constexpr (std::is_unsigned_v<T>) //uintX_t
            return std::numeric_limits<T>::max();
        if (str[0] == '-') //intX_t
            return std::numeric_limits<T>::min();
        return std::numeric_limits<T>::max();
    }
    return value;
}

template <class Point, class Base>
static std::optional<Point> stringToPoint(std::string_view str) {
    const size_t stick = str.find_first_of('|');
    if (stick == std::string::npos)
        return std::nullopt;
    const auto x = stringToNumber<Base>(str.substr(0, stick));
    const auto y = stringToNumber<Base>(str.substr(stick + 1));
    if (!x.has_value() || !y.has_value())
        return std::nullopt;
    return Point(x.value(), y.value());
}

// Unsigned int
std::optional<uint64_t> validator::toUint64(const std::string& str) {
    return stringToNumber<uint64_t>(str);
}
std::optional<uint32_t> validator::toUint32(const std::string& str) {
    return stringToNumber<uint32_t>(str);
}
std::optional<uint16_t> validator::toUint16(const std::string& str) {
    return stringToNumber<uint16_t>(str);
}
std::optional<uint8_t> validator::toUint8(const std::string& str) {
    return stringToNumber<uint8_t>(str);
}

// Signed int
std::optional<int64_t> validator::toInt64(const std::string& str) {
    return stringToNumber<int64_t>(str);
}
std::optional<int32_t> validator::toInt32(const std::string& str) {
    return stringToNumber<int32_t>(str);
}
std::optional<int16_t> validator::toInt16(const std::string& str) {
    return stringToNumber<int16_t>(str);
}
std::optional<int8_t> validator::toInt8(const std::string& str) {
    return stringToNumber<int8_t>(str);
}

//other
std::optional<TileCoord> validator::toTileCoord(const std::string& str) {
    return stringToPoint<TileCoord, int>(str);
}
std::optional<PixelCoord> validator::toPixelCoord(const std::string& str) {
    return stringToPoint<PixelCoord, float>(str);
}

std::optional<float> validator::toFloat(const std::string& str) {
    return stringToNumber<float>(str);
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
