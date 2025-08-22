#pragma once
#include <cstdint>
#include <string>

namespace utf8 {
    uint32_t fromConstCharToUint32(const char* s) noexcept;
    std::u32string fromConstCharToU32String(const char* cStr);
}
