#pragma once
#include <cstdint>
#include <optional>
#include <string_view>
#include "engine/coords/pixel_coord.hpp"
#include "engine/coords/tile_coord.hpp"
#include "engine/io/utf8/utf8.hpp"

namespace validator {
    template <class T>
    std::optional<T> stringToNumber(std::string_view str);

    template <class Point, class Base>
    std::optional<Point> stringToPoint(std::string_view str);

    std::optional<bool> toBool(const std::string_view str);

    template <class T>
    inline std::optional<T> to(const std::string& str) {
        if constexpr (std::is_same_v<T, std::string>) return str;
        else if constexpr (std::is_same_v<T, std::u32string>) return utf8::to_u32string(str);
        else if constexpr (std::is_same_v<T, uint64_t>) return stringToNumber<uint64_t>(str);
        else if constexpr (std::is_same_v<T, uint32_t>) return stringToNumber<uint32_t>(str);
        else if constexpr (std::is_same_v<T, uint16_t>) return stringToNumber<uint16_t>(str);
        else if constexpr (std::is_same_v<T, uint8_t>)  return stringToNumber<uint8_t>(str);
        else if constexpr (std::is_same_v<T, int64_t>)  return stringToNumber<int64_t>(str);
        else if constexpr (std::is_same_v<T, int32_t>)  return stringToNumber<int32_t>(str);
        else if constexpr (std::is_same_v<T, int16_t>)  return stringToNumber<int16_t>(str);
        else if constexpr (std::is_same_v<T, int8_t>)   return stringToNumber<int8_t>(str);
        else if constexpr (std::is_same_v<T, float>)    return stringToNumber<float>(str);
        else if constexpr (std::is_same_v<T, bool>)     return toBool(str);
        else if constexpr (std::is_same_v<T, TileCoord>)  return stringToPoint<TileCoord, int>(str);
        else if constexpr (std::is_same_v<T, PixelCoord>) return stringToPoint<PixelCoord, float>(str);
        else {
            static_assert(false, "Unsupported type for validator::to<T>");
            return std::nullopt;
        }
    }

    template <class T>
    inline std::optional<T> to(const std::u32string& u32str) {
        if constexpr (std::is_same_v<T, std::u32string>) return u32str;
        return to<T>(utf8::to_string(u32str));
    }
}
