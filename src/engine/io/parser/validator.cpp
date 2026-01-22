#include "validator.hpp"
//
#include <algorithm>
#include <charconv>
#include <limits>
#include <string_view>
#include <utility>

template <class T>
std::optional<T> validator::stringToNumber(std::string_view str) {
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

template std::optional<uint64_t> validator::stringToNumber(std::string_view str);
template std::optional<uint32_t> validator::stringToNumber(std::string_view str);
template std::optional<uint16_t> validator::stringToNumber(std::string_view str);
template std::optional<uint8_t>  validator::stringToNumber(std::string_view str);
//
template std::optional<int64_t> validator::stringToNumber(std::string_view str);
template std::optional<int32_t> validator::stringToNumber(std::string_view str);
template std::optional<int16_t> validator::stringToNumber(std::string_view str);
template std::optional<int8_t>  validator::stringToNumber(std::string_view str);

template std::optional<float> validator::stringToNumber<float>(std::string_view str);

template <class Point, class Base>
std::optional<Point> validator::stringToPoint(std::string_view str) {
    const size_t stick = str.find_first_of('|');
    if (stick == std::string::npos)
        return std::nullopt;
    const auto x = stringToNumber<Base>(str.substr(0, stick));
    const auto y = stringToNumber<Base>(str.substr(stick + 1));
    if (!x.has_value() || !y.has_value())
        return std::nullopt;
    return Point(x.value(), y.value());
}

template std::optional<TileCoord>  validator::stringToPoint<TileCoord, int>(std::string_view str);
template std::optional<PixelCoord> validator::stringToPoint<PixelCoord, float>(std::string_view str);

std::optional<bool> validator::toBool(std::string_view str) {
    if (str == "true" || str == "1")
        return true;
    if (str == "false" || str == "0")
        return false;
    return std::nullopt;
}
