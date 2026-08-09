#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include "engine/coords/pixel_coord.hpp"
#include "engine/coords/tile_coord.hpp"

namespace validator {
    template <class T>
    std::optional<T> stringToNumber(std::string_view str);

    template <class Point, class Base>
    std::optional<Point> stringToPoint(std::string_view str);

    std::optional<bool> toBool(std::string_view str);

    template <class T>
    inline std::optional<T> to(std::string_view str) {
        if constexpr (std::is_same_v<T, std::string>) return std::string(str);
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
}
