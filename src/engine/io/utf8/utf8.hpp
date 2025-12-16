#pragma once
#include <cstdint>
#include <string>

namespace utf8 {
    std::string to_string(const std::u32string& u32Str);
    char32_t to_char32_t(const char* s) noexcept;
    std::u32string to_u32string(const char* cStr);
    inline std::u32string to_u32string(const std::string str) {
        return to_u32string(str.c_str());
    }

    namespace priv {
        template <typename T>
        char32_t* fromUintToBuffer(char32_t* rNext, T unsignedValue) {
            static_assert(std::is_unsigned_v<T>, "T must be unsigned");
            auto _UVal_trunc = unsignedValue;
            do {
                *--rNext = static_cast<char32_t>(U'0' + _UVal_trunc % 10);
                _UVal_trunc /= 10;
            } while (_UVal_trunc != 0);
            return rNext;
        }
    }

    template<typename T>
    static std::u32string to_u32string(const T value) {
        static_assert(std::is_integral_v<T>, "T must be integral");
        char32_t buffer[21]; // can hold -2^63 and 2^64 - 1, plus NUL
        char32_t* const bufferEnd = std::end(buffer);
        char32_t* rNext = bufferEnd;

        if constexpr (std::is_unsigned_v<T>)
            rNext = priv::fromUintToBuffer(rNext, value);
        else {
            using UT = std::make_unsigned_t<T>;
            const auto unsignedValue = static_cast<UT>(value);
            if (value >= 0)
                rNext = priv::fromUintToBuffer(rNext, unsignedValue);
            else {
                rNext = priv::fromUintToBuffer(rNext, static_cast<UT>(UT(0U) - unsignedValue));
                *--rNext = U'-';
            }
        }
        return std::u32string(rNext, bufferEnd);
    }

    static std::u32string to_u32string(const float value) {
        return to_u32string(std::to_string(value));
    }
}
